#include "ToTextureRendererInstanced.h"
#include <iostream>
#include <sstream>

#include <GL/error.h>
#include <GL/framebuffer.h>
#include <GL/texture.h>
#include "GeometryGeneratorInstanced.h"
#include "math/vector.h"

#include "png_wrapper.h"

extern const char instanced_vs[];
extern const char object_render_flat_fs[];
extern const char compare_pixels_cs[];

#define PGA_GPU_CHECK_PERFORMANCE
#undef PGA_GPU_CHECK_PERFORMANCE

namespace PGA {
namespace GPU {


static const unsigned int VERTEX_POS_ARRAY = 0;
static const unsigned int VERTEX_NORMAL_ARRAY = 1;
static const unsigned int VERTEX_TEX_ARRAY = 2;
static const unsigned int MATRIX_ARRAY = 3;


#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B


static int countBlackPixels(PNG::image<std::uint32_t> image)
{
	int pixels = 0;

	typedef struct my_rgba_{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	}my_rgba;

	int w = image.getWidth();
	int h = image.getHeight();

	for (int index = 0; index < (w * h); ++index) {
		my_rgba* pixel = reinterpret_cast<my_rgba*>(&image.getData()[index]);
		if (pixel->r == 0 && pixel->g == 0 && pixel->b == 0 && pixel->a == 255) {
			pixels ++;
		}
	}
	return pixels;
}

void print_graka_mem_stats_header()
{
	std::cout << "Dedicated memory,";
	std::cout << "Total available,";
	std::cout << "current available,";
	std::cout << "Eviction count,";
	std::cout << "evicted memory" << std::endl;
}

void print_graka_mem_stats()
{
	int dedicated_memory = -1;
	int total_memory = -1;
	int current_memory = -1;
	int eviction_count = -1;
	int evicted_memory = -1;
	glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);
	glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
	glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &current_memory);
	glGetIntegerv(GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &eviction_count);
	glGetIntegerv(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicted_memory);

	std::cout << dedicated_memory << ",";
	std::cout << total_memory << ",";
	std::cout << current_memory << ",";
	std::cout << eviction_count << ",";
	std::cout << evicted_memory << std::endl;

}

ToTextureRendererInstanced::ToTextureRendererInstanced(const std::string& filename, int width, int height, const std::string& stats_filename)
	: _phi(0), _theta(0), _radius(0), _lookat(0),
	  _width(width), _height(height),
	  _aspect(static_cast<float>(_width) / static_cast<float>(_height)),
	  _fov(0), _znear(0), _zfar(0), _position(0),
	vs_object(GL::compileVertexShader(instanced_vs)),
	fs_object(GL::compileFragmentShader(object_render_flat_fs)),
	cs_compare(GL::compileComputeShader(compare_pixels_cs)),
	run(0),
	last_generation(0),
	move_time(0.0f),
	render_time(0.0f),
	compare_time(0.0f),
	num_shapes(0),
	geometry_on_device(false)
{
	// shader program to render the object from the grammar
	glAttachShader(prog_object, vs_object);
	glAttachShader(prog_object, fs_object);
	GL::linkProgram(prog_object);
	GL::checkError();

	// load the target image
	PNG::image< std::uint32_t > target = PNG::loadImage2D(filename.c_str());
	glBindTexture(GL_TEXTURE_2D, texture_target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data(target));
	glBindTexture(GL_TEXTURE_2D, 0);
	GL::checkError();

	target_pixels = countBlackPixels(target);

	glBindTexture(GL_TEXTURE_2D, texture_object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GL::checkError();

	// camera buffer
	glBindBuffer(GL_UNIFORM_BUFFER, camera_uniform_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(Camera::UniformBuffer), nullptr, GL_DYNAMIC_STORAGE_BIT);
	GL::checkError();

	// cull backfaces, but don't check the depth-buffer
	glEnable(GL_CULL_FACE);

	GL::checkError();

	// shader program to compare the pixels of the target and the object
	glAttachShader(prog_compare, cs_compare);
	GL::linkProgram(prog_compare);
	GL::checkError();

	texture_object_location = glGetUniformLocation(prog_compare, "texture_object_location");
	texture_target_location = glGetUniformLocation(prog_compare, "texture_target_location");

	texture_width_location = glGetUniformLocation(prog_compare, "texture_width");
	texture_height_location = glGetUniformLocation(prog_compare, "texture_height");

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, count_buffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * 2, nullptr, GL_DYNAMIC_STORAGE_BIT);

	// set up the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fb_object);

	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_object, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "error setting up framebuffer" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//print_graka_mem_stats();

	calcCamera();
#ifdef PGA_GPU_CHECK_PERFORMANCE
	if (!stats_filename.empty()) {
		stats_file.open(stats_filename);
	}
#endif

//	glEnable(GL_DEBUG_OUTPUT);
}


ToTextureRendererInstanced::~ToTextureRendererInstanced()
{
	stats_file.close();
}

void ToTextureRendererInstanced::writeCamera()
{
	glBindBuffer(GL_UNIFORM_BUFFER, camera_uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera::UniformBuffer), &camera);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0U, camera_uniform_buffer, 0U, sizeof(Camera::UniformBuffer));
	GL::checkError();
}

void ToTextureRendererInstanced::setupCamera(float fov, float znear, float zfar, float phi, float theta, float radius, const math::float3& lookat)
{
	_fov = fov;
	_znear = znear;
	_zfar = zfar;
	_phi = phi;
	_theta = theta;
	_radius = radius;
	_lookat = lookat;
	calcCamera();

	std::ofstream file("camera_PV.txt");
	file << camera.PV._11 << ", " << camera.PV._12 << ", " << camera.PV._13 << ", " << camera.PV._14 << std::endl;
	file << camera.PV._21 << ", " << camera.PV._22 << ", " << camera.PV._23 << ", " << camera.PV._24 << std::endl;
	file << camera.PV._31 << ", " << camera.PV._32 << ", " << camera.PV._33 << ", " << camera.PV._34 << std::endl;
	file << camera.PV._41 << ", " << camera.PV._42 << ", " << camera.PV._43 << ", " << camera.PV._44 << std::endl;
}


void ToTextureRendererInstanced::resetBuffers()
{
	for (int i = 0; i < num_shapes; ++i) {
		mbo_size[i] = 0;
	}
}

void ToTextureRendererInstanced::calcCamera()
{

	const float s2 = 1.0f / std::tan(_fov * 0.5f);
	const float s1 = s2 / _aspect;
	const float z1 = (_zfar + _znear) / (_zfar - _znear);
	const float z2 = -2.0f * _znear * _zfar / (_zfar - _znear);

	camera.P = math::float4x4(s1, 0.0f, 0.0f, 0.0f,
	                      0.0f, s2, 0.0f, 0.0f,
	                      0.0f, 0.0f, z1, z2,
	                      0.0f, 0.0f, 1.0f, 0.0f);
	camera.P_inv = math::inverse(camera.P);

	float cp = math::cos(_phi);
	float sp = math::sin(_phi);
	float ct = math::cos(_theta);
	float st = math::sin(_theta);

	math::float3 w = -math::float3(ct * cp, st, ct * sp);
	math::float3 v = math::float3(-st * cp, ct, -st * sp);
	math::float3 u = cross(v, w);

	_position = _lookat - _radius * w;

	camera.V = math::affine_float4x4(u.x, u.y, u.z, -dot(u, _position),
									 v.x, v.y, v.z, -dot(v, _position),
									 w.x, w.y, w.z, -dot(w, _position));

	camera.V_inv = math::affine_float4x4(u.x, v.x, w.x, _position.x,
										 u.y, v.y, w.y, _position.z,
										 u.z, v.z, w.z, _position.y);

	camera.position = _position;

	camera.PV = camera.P * camera.V;
	camera.PV_inv = camera.V_inv * camera.P_inv;
}

int ToTextureRendererInstanced::copyMatricesToDevice(GeometryBufferInstanced& buffer)
{
	glBindVertexArray(0);

	int matrices = 0;

	for (int i = 0; i < buffer.getNumShapes(); ++i) {
		int num_mats = buffer.getMatrixBuffer(i).size();
		num_instances[i] = num_mats;
		if (num_mats == 0) {
			continue;
		}


		std::vector<math::float3x4> tmp;
		tmp.resize(num_mats);
		buffer.getMatrixBuffer(i).copy(&tmp[0], 0, num_mats);

		std::vector<math::float4x4> m;
		for (int i = 0; i < num_mats; ++i) {
			// openGL requires a transposed matrix
			math::float4x4 mat(tmp[i]._11, tmp[i]._21, tmp[i]._31, 0.0f,
							   tmp[i]._12, tmp[i]._22, tmp[i]._32, 0.0f,
							   tmp[i]._13, tmp[i]._23, tmp[i]._33, 0.0f,
							   tmp[i]._14, tmp[i]._24, tmp[i]._34, 1.0f);

			m.push_back(mat);
			matrices ++;
		}
		static const unsigned int VERTEX_POS_ARRAY = 0;
		static const unsigned int VERTEX_NORMAL_ARRAY = 1;
		static const unsigned int VERTEX_TEX_ARRAY = 2;
		static const unsigned int MATRIX_ARRAY = 3;

		int target_buffer_size = sizeof(math::float4x4) * num_mats;
		int current_buffer_size = mbo_size[i];

		if (current_buffer_size < target_buffer_size) {

			glBindVertexArray(vaos[i]);

			mbos[i] = GL::Buffer();

			glBindBuffer(GL_ARRAY_BUFFER, mbos[i]);

			for (unsigned int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(MATRIX_ARRAY + i);
				glVertexAttribPointer(MATRIX_ARRAY + i, 4, GL_FLOAT, GL_FALSE, sizeof(math::float4x4),
					(const void*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor(MATRIX_ARRAY + i, 1);
			}

			glBindVertexArray(0);
			if (current_buffer_size == 0) {
				current_buffer_size = target_buffer_size;
			}
			else {
				do {
					current_buffer_size = current_buffer_size * 2;
				}while(current_buffer_size < target_buffer_size);
			}
			glBufferStorage(GL_ARRAY_BUFFER, target_buffer_size, &m[0], GL_DYNAMIC_STORAGE_BIT);
			mbo_size[i] = target_buffer_size;
			GL::checkError();
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, mbos[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, target_buffer_size, &m[0]);
			GL::checkError();
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return matrices;

}


void ToTextureRendererInstanced::copyGeometryToDevice(GeometryBufferInstanced& buffer)
{

	glBindVertexArray(0);

	num_shapes = buffer.getNumShapes();
	vaos.clear();
	vbos.clear();
	num_vertices.clear();
	num_instances.clear();
	mbo_size.clear();
	mbos.clear();

	for (int i = 0; i < num_shapes; ++i) {

		// prepare buffers

		vaos.push_back(GL::VertexArray());
		vbos.push_back(GL::Buffer());
		mbos.push_back(GL::Buffer());

		std::vector<PGA::GeneratedVertex> geom;
		glBindVertexArray(vaos[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

		glEnableVertexAttribArray(VERTEX_POS_ARRAY);
		glVertexAttribPointer(VERTEX_POS_ARRAY, 4, GL_FLOAT, GL_FALSE, sizeof(PGA::GeneratedVertex), 0);
		GL::checkError();

		glEnableVertexAttribArray(VERTEX_NORMAL_ARRAY);
		glVertexAttribPointer(VERTEX_NORMAL_ARRAY, 4, GL_FLOAT, GL_FALSE, sizeof(PGA::GeneratedVertex), (void*)offsetof(PGA::GeneratedVertex, n));
		GL::checkError();

		glEnableVertexAttribArray(VERTEX_TEX_ARRAY);
		glVertexAttribPointer(VERTEX_TEX_ARRAY, 2, GL_FLOAT, GL_FALSE, sizeof(PGA::GeneratedVertex), (void*)offsetof(PGA::GeneratedVertex, tex));
		GL::checkError();

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GL::checkError();

		// copy the vertices and indices to the device
		glBindVertexArray(0);

		PGA::ObjShape &shape = *buffer.getShape(i);

		int vbo_size = shape.vertices.size() * sizeof(PGA::GeneratedVertex);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
		glBufferStorage(GL_ARRAY_BUFFER, vbo_size, &shape.vertices[0], 0U);

		GL::checkError();

		num_vertices.push_back(shape.vertices.size());
		num_instances.push_back(0);
		mbo_size.push_back(0);
	}

	GL::checkError();

}


int ToTextureRendererInstanced::divUp(int value, int blocksize)
{
	return (value + blocksize - 1) / blocksize;
}

void ToTextureRendererInstanced::compare(GeometryBufferInstanced* buffer, int& good_pixels, int& bad_pixels, int generation)
{

#ifdef PGA_GPU_CHECK_PERFORMANCE
	auto t0 = std::chrono::high_resolution_clock::now();
#endif /* PGA_GPU_CHECK_PERFORMANCE */

	if (geometry_on_device == false) {
		copyGeometryToDevice(*buffer);
		geometry_on_device = true;
#ifdef PGA_GPU_CHECK_PERFORMANCE
		stats_file <<  "move_time, render_time, compare_time, matrices " << std::endl;
		//print_graka_mem_stats_header();
#endif /* PGA_GPU_CHECK_PERFORMANCE */
	}
	int mats = copyMatricesToDevice(*buffer);

#ifdef PGA_GPU_CHECK_PERFORMANCE
	if (last_generation != generation) {
		last_generation = generation;
		//print_graka_mem_stats();
	}
#endif /* PGA_GPU_CHECK_PERFORMANCE */


#ifdef PGA_GPU_CHECK_PERFORMANCE
	// call glFinish to actually do the operation on the gpu
	glFinish();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	move_time = time_span.count();
#endif /* PGA_GPU_CHECK_PERFORMANCE */

	glUseProgram(prog_object);

	GL::checkError();

	writeCamera();

	glBindFramebuffer(GL_FRAMEBUFFER, fb_object);

	GL::checkError();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepthf(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	GL::checkError();

	math::float4 color(0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4fv(0, 1, &color.x);

	GL::checkError();
	for (int i = 0; i < num_shapes; ++i) {
		glBindVertexArray(vaos[i]);
		glDrawArraysInstanced(GL_TRIANGLES, 0, num_vertices[i], num_instances[i]);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL::checkError();


#ifdef PGA_GPU_CHECK_PERFORMANCE
	// call glFinish to actually do the operation on the gpu
	glFinish();
	auto t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	render_time = time_span.count();
#endif /* PGA_GPU_CHECK_PERFORMANCE */

	// compare the pixel in a compute shader
	glUseProgram(prog_compare);

	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, count_buffer);

	// reset the buffer values
	GLuint result[2] = {0};
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint) * 2, result);


	// bind the rendered image and the target image as textures
	glBindImageTexture(0, texture_target, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glUniform1i(texture_target_location, 0);
	glBindImageTexture(1, texture_object,0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glUniform1i(texture_object_location, 1);

//	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_target);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, texture_object);

	glUniform1ui(texture_width_location, _width);
	glUniform1ui(texture_height_location, _height);

	int blocks_x = divUp(_height, 8);
	int blocks_y = divUp(_width, 8);

	// start the compute shader
	glDispatchCompute(blocks_x, blocks_y, 1);

	// copy the result back
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(unsigned int) * 2, result);

	good_pixels = result[0];
	bad_pixels = result[1];

	if (good_pixels > (_width * _height) || bad_pixels > (_width * _height)) {
		std::cout << "huh?" << std::endl;
	}


//	std::stringstream filename("");
//	filename << "out_" << generation << "-" << run << "_t_" << target_pixels << "_g_" << good_pixels << "_b_" << bad_pixels << ".png";
//	saveFbToImage(filename.str());

#ifdef PGA_GPU_CHECK_PERFORMANCE
	glFinish();
	auto t3 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t3 - t2);

	compare_time = time_span.count();
	stats_file <<  move_time * 1000.0f << "," <<  render_time * 1000.0f << "," << compare_time * 1000.0f << "," << mats << std::endl;


#endif /* PGA_GPU_CHECK_PERFORMANCE */

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL::checkError();

	run ++;

}

void ToTextureRendererInstanced::countPixels(int& good, int& bad)
{
	typedef struct my_rgba_{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	}my_rgba;

	good = 0;
	bad = 0;

	std::vector<my_rgba> img;
	img.resize(_width * _height);
	glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(&img[0]));
	for (int i = 0; i < img.size(); ++i) {
		if (img[i].r == 255) {
			bad ++;
		}
		else if (img[i].g == 255) {
			good ++;
		}
	}
}

void ToTextureRendererInstanced::saveFbToImage(const std::string filename)
{
	PNG::image<std::uint32_t> img(_width, _height);

	glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data(img));

	PNG::saveImage(filename.c_str(), img);

}




} // namespace GPU

}  // namespace PGA
