


#ifndef INCLUDED_PLATFORM_X11_DISPLAY
#define INCLUDED_PLATFORM_X11_DISPLAY

#pragma once

#include <utility>

#include "X11/platform.h"


namespace X11
{
	class Display
	{
	private:
		Display(const Display&);
		Display& operator =(const Display&);

		::Display* display;

	public:
		Display(::Display* display)
			: display(display)
		{
		}

		Display(Display&& d)
			: display(d.display)
		{
			d.display = 0;
		}

		~Display();

		Display& operator =(Display&& d)
		{
			using std::swap;
			swap(display, d.display);
			return *this;
		}

		operator ::Display*() const { return display; }
	};

	Display openDisplay();
}

#endif  // INCLUDED_PLATFORM_X11_DISPLAY

