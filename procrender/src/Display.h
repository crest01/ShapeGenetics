


#ifndef INCLUDED_DISPLAY
#define INCLUDED_DISPLAY

#pragma once

#include <string>
#include <chrono>

#include <GL/buffer.h>
#include <GL/shader.h>
#include <GL/texture.h>
#include <GL/vertex_array.h>
#include <GL/framebuffer.h>

#include <GL/platform/Context.h>
#include <GL/platform/Window.h>


#include "Navigator.h"


class Config;
class PerformanceMonitor;


class myDisplay : private virtual GL::platform::DisplayHandler
{
private:
	GL::platform::Window window;
	GL::platform::Context context;
	GL::platform::context_scope<GL::platform::Window> ctx;

	int buffer_width;
	int buffer_height;

	//GL::VertexArray vao;

	//GL::Texture color_buffer;

	//GL::Sampler color_buffer_sampler;

	//GL::VertexShader vs;
	//GL::FragmentShader fs;
	//GL::Program prog;


	void close();
	void destroy() {}
	void move(int x, int y);
	void resize(int width, int height);

public:
	myDisplay(const myDisplay&) = delete;
	myDisplay& operator =(const myDisplay&) = delete;

	myDisplay(const Config& config);
	void setViewport() const;
	float getAspect() const;
	void swapBuffers();

	void attach(GL::platform::MouseInputHandler* mouse_input);
	void attach(GL::platform::KeyboardInputHandler* keyboard_input);

	void save(Config& config) const;
};

#endif  // INCLUDED_DISPLAY
