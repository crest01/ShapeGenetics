#include "InstancedRenderer.h"
#include <iostream>
#include <cstddef>

#include "Config.h"

#include <GL/error.h>
#include <GL/platform/Application.h>
#include "png_wrapper.h"
#include "math/matrix.h"

static const unsigned int VERTEX_POS_ARRAY = 0;
static const unsigned int VERTEX_NORMAL_ARRAY = 1;
static const unsigned int VERTEX_TEX_ARRAY = 2;
static const unsigned int MATRIX_ARRAY = 3;

extern const char instanced_vs[];
extern const char lambert_fs[];
extern const char voxels_vs[];
extern const char flatpoints_fs[];

InstancedRenderer::InstancedRenderer(Config& config, Scene& scene)
	: camera(math::constants<float>::pi() * 0.3333f, 1.0f, 100.0f),
	display(config),
	vs(GL::compileVertexShader(instanced_vs)),
	fs(GL::compileFragmentShader(lambert_fs)),
	vs_voxels(GL::compileVertexShader(voxels_vs)),
	fs_voxels(GL::compileFragmentShader(flatpoints_fs)),
	scene(scene),
	generate(true),
	geometry_on_device(false),
	display_voxels(false),
	num_shapes(0),
	num_instances(0),
	num_voxels(0),
	voxel_vbo_size(0),
	first(true)
{
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	GL::linkProgram(prog);

	glAttachShader(prog_voxels, vs_voxels);
	glAttachShader(prog_voxels, fs_voxels);
	GL::linkProgram(prog_voxels);

	GL::checkError();

	glBindBuffer(GL_UNIFORM_BUFFER, camera_uniform_buffer);
	glBufferStorage(GL_UNIFORM_BUFFER, sizeof(Camera::UniformBuffer), nullptr, GL_DYNAMIC_STORAGE_BIT);

	GL::checkError();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	GL::checkError();

	glBindVertexArray(voxel_vao);
	glBindVertexBuffer(0U, voxel_vbo, 0U, sizeof(math::float4));
	glVertexAttribFormat(0U, 4, GL_FLOAT, GL_FALSE, 0U);
	glVertexAttribBinding(0U, 0U);
	glEnableVertexAttribArray(0U);
	GL::checkError();

}

void  InstancedRenderer::writeCamera()
{
	Camera::UniformBuffer camera_params;
	camera.writeUniformBuffer(&camera_params, display.getAspect());
	glBindBuffer(GL_UNIFORM_BUFFER, camera_uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera::UniformBuffer), &camera_params);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0U, camera_uniform_buffer, 0U, sizeof(Camera::UniformBuffer));

	GL::checkError();
}

void InstancedRenderer::copyMatricesToDevice(PGA::GeometryBufferInstanced& buffer)
{
	glBindVertexArray(0);

	for (int i = 0; i < buffer.getNumShapes(); ++i) {
		int num_mats = buffer.getMatrixBuffer(i).size();
		std::cout << "copy " << num_mats << " matrices for shape " << i << std::endl;
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
		}

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

			glBufferStorage(GL_ARRAY_BUFFER, target_buffer_size, &m[0], GL_DYNAMIC_STORAGE_BIT);
			mbo_size[i] = target_buffer_size;
			GL::checkError();
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, mbos[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, target_buffer_size, &m[0]);
			GL::checkError();
		}
		num_instances[i] = num_mats;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


void InstancedRenderer::copyGeometryToDevice(PGA::GeometryBufferInstanced& buffer)
{


	glBindVertexArray(0);

	num_shapes = buffer.getNumShapes();
	vaos.clear();
	vbos.clear();
	num_vertices.clear();
	num_instances.clear();
	mbos.clear();
	mbo_size.clear();

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

	std::cout << "copy " << num_shapes << " shapes" << std::endl;


	GL::checkError();

}

void InstancedRenderer::copyVoxelCoordsToDevice()
{

	std::vector<math::float4> centers;

	scene.getVoxels(centers);

	num_voxels = static_cast<GLsizei>(centers.size());

/*	for (int i = 0; i < num_voxels; ++i) {
		math::float4 coords = centers[i];
		std::cout << "voxel coords (" << coords.x << "," << coords.y << "," << coords.z << "), type=" << coords.w << std::endl;
	}
*/
	glBindVertexArray(0);

	if (num_voxels == 0) {
		std::cout << "Warning no geometry generated\n";
	}
	else {
		if (voxel_vbo_size < (num_voxels * sizeof(math::float4))) {
			voxel_vbo_size = num_voxels * sizeof(math::float4);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			voxel_vbo = GL::Buffer();
			glBindBuffer(GL_ARRAY_BUFFER, voxel_vbo);
			glBufferStorage(GL_ARRAY_BUFFER, voxel_vbo_size, &centers[0], GL_DYNAMIC_STORAGE_BIT);

			// register the new buffer for the Voxel VAO
			glBindVertexArray(voxel_vao);
			glBindVertexBuffer(0U, voxel_vbo, 0U, sizeof(math::float4));
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, voxel_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, centers.size()*sizeof(math::float4), &centers[0]);
		}
		GL::checkError();
	}
	std::cout << "copy " << num_voxels << " voxels" << std::endl;

}


void InstancedRenderer::render()
{

	glClearColor(0.0f, 0.2f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepthf(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	bool save_image = false;

	if (display_voxels) {
		if (geometry_on_device == false) {
			copyVoxelCoordsToDevice();
			geometry_on_device = true;
		}


		glUseProgram(prog_voxels);
		writeCamera();
		glBindVertexArray(voxel_vao);
		glDrawArrays(GL_POINTS, 0, num_voxels);
	}
	else {

		if (batchmode)
		{
			scene.recombine(false);
			generate = true;
			save_image = true;
			scene.setCameraToEvalPoint(navigator);
		}
		if (generate)
		{
			double t = scene.generate();
			generate = false;
			updateBuffers = true;
		}

		PGA::GeometryBufferInstanced& buffer = scene.getGeometryBuffer();

		// do this only once
		if (geometry_on_device == false) {
			copyGeometryToDevice(buffer);
			geometry_on_device = true;
		}


		if (updateBuffers) {
			copyMatricesToDevice(buffer);
			updateBuffers = false;
		}

		glUseProgram(prog);
		writeCamera();

		for (int i = 0; i < num_shapes; ++i) {
			if (num_instances[i] == 0) {
				continue;
			}
			glUniform4fv(0, 1, &buffer.getColor(i).x);

			glBindVertexArray(vaos[i]);

	//		glDrawElementsInstanced(GL_TRIANGLES, num_indices[i], GL_UNSIGNED_INT, nullptr, num_instances[i]);
			glDrawArraysInstanced(GL_TRIANGLES, 0, num_vertices[i], num_instances[i]);
		}

	}

	GL::checkError();

	if (save_image) {
		saveFbToImage(scene.getTargetImageFilename());
	}

	display.swapBuffers();


	if (batchmode) {
		GL::platform::quit();
	}

}

void InstancedRenderer::attach(Navigator* navigator)
{
	this->navigator = navigator;
	camera.attach(navigator);
}

void InstancedRenderer::attach(GL::platform::MouseInputHandler* mouse_input)
{
	display.attach(mouse_input);
}

void InstancedRenderer::attach(GL::platform::KeyboardInputHandler* keyboard_input)
{
	display.attach(keyboard_input);
}

void InstancedRenderer::save(Config& config) const
{
	//config.saveString("current_renderer", renderers.empty() ? "" : std::get<0>(renderers[current_plugin]));
	display.save(config);
}

void InstancedRenderer::saveFbToImage(const std::string filename)
{
	int width = 800;
	int height = 600;

	PGA::PNG::image<std::uint32_t> img(width, height);

	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data(img));

	PGA::PNG::saveImage(filename.c_str(), img);

}


