


#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "../Renderer.h"
#include "../DisplayHandler.h"
#include "../InputHandler.h"

#include "X11Display.h"
#include "X11WindowHandle.h"
#include "X11GLWindow.h"


namespace
{
	static X11::Display display = X11::openDisplay();
	static std::unordered_map<::Window, X11::GLWindow*> window_map;

	GLXFBConfig findFBConfig(::Display* display, bool depth_buffer)
	{
		static int attribs[] = {
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			GLX_ALPHA_SIZE      , 8,
			GLX_DEPTH_SIZE      , depth_buffer ? 24 : 0,
			GLX_STENCIL_SIZE    , depth_buffer ? 8 : 0,
			GLX_DOUBLEBUFFER    , True,
			None
		};

		int num_configs;
		std::unique_ptr<GLXFBConfig[], X11::deleter> configs(glXChooseFBConfig(display, DefaultScreen(display), attribs, &num_configs));

		if (configs == nullptr)
			throw std::runtime_error("no matching GLXFBConfig.");

		return configs[0];
	}

	X11::WindowHandle createWindow(Display* display, int width, int height, XVisualInfo* vi, Colormap colormap)
	{
		XSetWindowAttributes swa;
		swa.colormap = colormap;
		swa.background_pixmap = None;
		swa.border_pixel = 0;
		swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

		return X11::createWindow(display, RootWindow(display, vi->screen), 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
	}
}

namespace X11
{
	GLWindow::GLWindow(const char* title, int width, int height, bool depth_buffer, bool vsync, int major, int minor)
		: fb_config(findFBConfig(display, depth_buffer)),
		  vi(glXGetVisualFromFBConfig(display, fb_config)),
		  colormap(createColorMap(display, RootWindow(static_cast< ::Display*>(display), vi->screen), vi->visual, AllocNone)),
		  window(::createWindow(display, width, height, vi.get(), colormap)),
		  context(createGLContext(display, fb_config, major, minor)),
		  display_handler(nullptr),
		  keyboard_handler(nullptr),
		  mouse_handler(nullptr)
	{
		window_map[window] = this;

		Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", False);
		XSetWMProtocols(display, window, &wmDelete, 1);

		XMapWindow(display, window);

		glXMakeCurrent(display, window, context);

		GLWindow::title(title);

		XSync(display, False);
	}

	void GLWindow::title(const char* title)
	{
		XStoreName(display, window, title);
		Atom format_utf8 = XInternAtom(display, "UTF8_STRING", False);
		Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
		XChangeProperty(display, window, net_wm_name, format_utf8, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(title), std::strlen(title));
	}

	void GLWindow::attach(Framework::DisplayHandler* handler)
	{
		display_handler = handler;
	}

	void GLWindow::attach(Framework::MouseInputHandler* handler)
	{
		mouse_handler = handler;
	}

	void GLWindow::attach(Framework::KeyboardInputHandler* handler)
	{
		keyboard_handler = handler;
	}

	void GLWindow::swapBuffers()
	{
		glXSwapBuffers(display, window);
	}

	bool GLWindow::handleEvent(const XEvent& event)
	{
		switch (event.type)
		{
		case ButtonPress:
			if (mouse_handler)
				mouse_handler->buttonDown(static_cast<Framework::Button>(1 << (event.xbutton.button - 1)), event.xbutton.x, event.xbutton.y);
			break;

		case ButtonRelease:
			if (mouse_handler)
				mouse_handler->buttonUp(static_cast<Framework::Button>(1 << (event.xbutton.button - 1)), event.xbutton.x, event.xbutton.y);
			break;

		case MotionNotify:
			if (mouse_handler)
				mouse_handler->mouseMove(event.xmotion.x, event.xmotion.y);
			break;

		case KeyPress:
			if (keyboard_handler)
				//keyboard_handler->keyDown(static_cast<Framework::Key>(XKeycodeToKeysym(display, event.xkey.keycode, 0)), (event.xkey.state & ShiftMask) != 0);
				keyboard_handler->keyDown(static_cast<Framework::Key>(XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0)));//, (event.xkey.state & ShiftMask) != 0);
			break;

		case KeyRelease:
			if (keyboard_handler)
			//keyboard_handler->keyUp(static_cast<Framework::Key>(XKeycodeToKeysym(display, event.xkey.keycode, 0)), (event.xkey.state & ShiftMask) != 0);
				keyboard_handler->keyUp(static_cast<Framework::Key>(XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0)));//, (event.xkey.state & ShiftMask) != 0);
			break;

		case ConfigureNotify:
			if (display_handler)
				display_handler->resize(event.xconfigure.width, event.xconfigure.height);
			break;

		case ClientMessage:
			{
				Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
				if (event.xclient.data.l[0] == wmDeleteMessage)
					return true;
			}
			break;
		}

		return false;
	}

	void run(Framework::Renderer& renderer, Framework::ConsoleHandler* console_handler)
	{
		XEvent event;
		bool quit = false;

		while (!quit)
		{
			while (XPending(display) > 0)
			{
				XNextEvent(display, &event);

				if (X11::GLWindow* window = window_map[event.xany.window])
					quit = window->handleEvent(event);
			}

			renderer.render();
		}
	}
}
