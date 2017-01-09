#ifndef INCLUDED_INSTANCED_RENDERER
#define INCLUDED_INSTANCED_RENDERER

#pragma once

#include <GL/platform/Renderer.h>
#include <GL/shader.h>
#include <GL/vertex_array.h>

#include "Display.h"
#include "PerspectiveCamera.h"
#include "RendererInput_IF.h"

#include "Scene.h"
#include <random>

class Config;
class InstancedRenderer : public GL::platform::Renderer, public RendererInput_IF
{
	PerspectiveCamera camera;
	Navigator*		navigator;
	myDisplay display;

	GL::VertexArray	voxel_vao;
	GL::Buffer		voxel_vbo;
	GLsizei			voxel_vbo_size;
	GLsizei			num_voxels;


	GL::VertexShader vs_voxels;
	GL::FragmentShader fs_voxels;
	GL::Program prog_voxels;


	GL::VertexShader vs;
	GL::FragmentShader fs;
	GL::Program prog;

	GL::Buffer camera_uniform_buffer;

	std::vector<GL::VertexArray> vaos;
	std::vector<GL::Buffer> vbos;
	std::vector<int> num_vertices;
	GLsizei num_shapes;

	std::vector<GL::Buffer> mbos;
	std::vector<int> num_instances;
	std::vector<int> mbo_size;

	bool geometry_on_device;
	Scene& scene;

	void writeCamera();
	bool generate;
	bool updateBuffers;
	bool display_voxels;
	bool batchmode;
	bool first;


	math::float4 colors[2];

	void copyGeometryToDevice(PGA::GeometryBufferInstanced& buffer);

	void copyMatricesToDevice(PGA::GeometryBufferInstanced& buffer);

	void copyVoxelCoordsToDevice();

	void saveFbToImage(const std::string filename);

public:
	InstancedRenderer(Config& config, Scene& scene);
	void render();

	void setBatchMode(bool batchmode) { this->batchmode = batchmode; }
	void regenerate()
	{
		generate = true;
		updateBuffers = true;
		geometry_on_device = false;
	}
	void toggleDisplayVoxels()
	{
		display_voxels = !display_voxels;
		updateBuffers = true;
		geometry_on_device = false;
	}

	void screenshot() { saveFbToImage("out.png"); }

	void attach(Navigator* navigator);
	void attach(GL::platform::MouseInputHandler* mouse_input);
	void attach(GL::platform::KeyboardInputHandler* keyboard_input);

	void save(Config& config) const;
};

#endif //INCLUDED_INSTANCED_RENDERER
