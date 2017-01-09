


#ifndef INCLUDED_PLATFORM_X11_WINDOW_HANDLE
#define INCLUDED_PLATFORM_X11_WINDOW_HANDLE

#pragma once

#include <utility>

#include "platform.h"


namespace X11
{
	class WindowHandle
	{
	private:
		WindowHandle(const WindowHandle&);
		WindowHandle& operator =(const WindowHandle&);

		::Display* display;
		::Window window;

	public:
		WindowHandle()
			: display(nullptr),
			  window(0)
		{
		}

		WindowHandle(::Display* display, ::Window window)
			: display(display),
			  window(window)
		{
		}

		WindowHandle(WindowHandle&& w)
			: display(w.display),
			  window(w.window)
		{
			w.window = 0;
		}

		~WindowHandle()
		{
			if (window)
				XDestroyWindow(display, window);
		}

		WindowHandle& operator =(WindowHandle&& w)
		{
			using std::swap;
			display = w.display;
			swap(window, w.window);
			return *this;
		}

		operator ::Window() const { return window; }
	};

	inline WindowHandle createWindow(::Display* display, ::Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int window_class, ::Visual* visual, unsigned long valuemask, XSetWindowAttributes* attributes)
	{
		return WindowHandle(display, XCreateWindow(display, parent, x, y, width, height, border_width, depth, window_class, visual, valuemask, attributes));
	}
}

#endif  // INCLUDED_PLATFORM_X11_WINDOW_HANDLE

