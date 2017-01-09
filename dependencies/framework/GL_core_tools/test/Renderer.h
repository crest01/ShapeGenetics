


#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#pragma once

#include <GL/gl.h>

#include <GL/platform/Renderer.h>
#include <GL/platform/Context.h>
#include <GL/platform/Window.h>
#include <GL/platform/DefaultDisplayHandler.h>

#include <GL/shader.h>
#include <GL/vertex_array.h>
#include <GL/texture.h>
#include <GL/sampler.h>


class Renderer : public virtual GL::platform::Renderer, private GL::platform::DefaultDisplayHandler
{
	GL::platform::Window window;
	GL::platform::Context context;
	GL::platform::context_scope<GL::platform::Window> ctx;

	int buffer_width;
	int buffer_height;

	GL::VertexArray vao;

	GL::FragmentShader fs;
	GL::VertexShader vs;
	GL::Program prog;

	GL::Texture tex;
	GL::Sampler sampler;


	void resize(int width, int height) override;

public:
	Renderer();

	void attach(GL::platform::MouseInputHandler* mouse_input);
	void attach(GL::platform::KeyboardInputHandler* keyboard_input);

	void render();
};

#endif  // INCLUDED_RENDERER
