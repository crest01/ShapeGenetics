


#ifndef INCLUDED_PLATFORM_X11_GL_WINDOW
#define INCLUDED_PLATFORM_X11_GL_WINDOW

#pragma once

#include <memory>

#include "X11/platform.h"

#include "x11_ptr.h"
#include "X11WindowHandle.h"
#include "X11ColormapHandle.h"
#include "X11GLContext.h"


namespace X11
{
	class GLWindow
	{
	friend void run(Framework::Renderer& renderer, Framework::ConsoleHandler* console_handler);
	private:
		GLXFBConfig fb_config;

		GLContext context;

		std::unique_ptr<XVisualInfo, X11::deleter> vi;
		ColormapHandle colormap;
		WindowHandle window;

		Framework::DisplayHandler* display_handler;
		Framework::KeyboardInputHandler* keyboard_handler;
		Framework::MouseInputHandler* mouse_handler;

		bool handleEvent(const XEvent& event);

	public:
		GLWindow(const char* title, int width = 800, int height = 600, bool depth_buffer = true, bool vsync = false, int major = 3, int minor = 3);

		void title(const char* title);

		void attach(Framework::DisplayHandler* display_handler);
		void attach(Framework::MouseInputHandler* mouse_input);
		void attach(Framework::KeyboardInputHandler* keyboard_input);

		void swapBuffers();
	};

  void run(Framework::Renderer& renderer, Framework::ConsoleHandler* console_handler = nullptr);
}

#endif  // INCLUDED_PLATFORM_X11_GL_WINDOW

