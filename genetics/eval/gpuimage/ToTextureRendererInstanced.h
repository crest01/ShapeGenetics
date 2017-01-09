#ifndef INCLUDED_TO_TEXTURE_RENDERER_INSTANCED
#define INCLUDED_TO_TEXTURE_RENDERER_INSTANCED

#pragma once

#include <GL/shader.h>
#include <GL/vertex_array.h>
#include <GL/framebuffer.h>
#include <GL/texture.h>
#include <GL/buffer.h>
#include "math/matrix.h"
#include "GeometryBufferInstanced.h"

namespace PGA {

namespace GPU {

namespace Camera {

#ifndef WIN32
#define __declspec(x) x
#define align(x) alignas(x)
#endif


struct UniformBuffer
{
	__declspec(align(64)) math::affine_float4x4 V;
	__declspec(align(64)) math::affine_float4x4 V_inv;
	__declspec(align(64)) math::float4x4 P;
	__declspec(align(64)) math::float4x4 P_inv;
	__declspec(align(64)) math::float4x4 PV;
	__declspec(align(64)) math::float4x4 PV_inv;
	__declspec(align(16)) math::float3 position;
};
} // namespace Camera



class ToTextureRendererInstanced
{

	// camera data
	float _phi;
	float _theta;
	float _radius;
	math::float3 _lookat;

	int   _width;
	int   _height;
	float _aspect;
	float _fov;
	float _znear;
	float _zfar;
	math::float3 _position;

	// the target image as texture
	GL::Texture			texture_target;
	GLint				texture_target_location;

	// the shaders to render the object from the grammar
	GL::VertexShader 	vs_object;
	GL::FragmentShader 	fs_object;
	GL::Program 		prog_object;

	// framebuffer and texture for the rendered object
	GL::Framebuffer 	fb_object;
	GL::Texture			texture_object;
	GLint				texture_object_location;
	GLuint				depthbuffer;


	GLint				texture_width_location;
	GLint				texture_height_location;

	// the shaders to compare the rendered object to the image
	GL::ComputeShader	cs_compare;
//	vs_textured_quad;
//	GL::FragmentShader 	fs_compare;
	GL::Program 		prog_compare;

	// final framebuffer
	GL::Buffer			count_buffer;
//	GL::Texture			texture_out;

	bool 	geometry_on_device;
	float	move_time;
	float	render_time;
	float 	compare_time;
	int 	last_generation;

	std::vector<GL::VertexArray> vaos;
	std::vector<GL::Buffer> vbos;
	std::vector<GL::Buffer> mbos;
	std::vector<int> num_vertices;
	std::vector<int> num_instances;
	int num_shapes;

	std::vector<int> mbo_size;

	int	target_pixels;

	std::ofstream stats_file;

	int run;

	GL::Buffer camera_uniform_buffer;
	Camera::UniformBuffer camera;

	void copyGeometryToDevice(PGA::GeometryBufferInstanced& buffer);

	int copyMatricesToDevice(PGA::GeometryBufferInstanced& buffer);


	void writeCamera();
	void calcCamera();
	void saveFbToImage(const std::string filename);
	void countPixels(int& good, int& bad);

	int divUp(int value, int blocks);

public:
	ToTextureRendererInstanced(const std::string& filename, int width = 800, int height = 600, const std::string& stats_filename = "perfstats.csv");
	~ToTextureRendererInstanced();

	void setupCamera(float fov, float znear, float zfar, float phi, float theta, float radius, const math::float3& lookat);

	void resetBuffers();

	int getTargetPixels()
	{
		return target_pixels;
	}

	void compare(GeometryBufferInstanced* buffer, int& good_pixels, int& bad_pixels, int generation);
};

} // namespace GPU
}  // namespace PGA

#endif //INCLUDED_RENDERER
