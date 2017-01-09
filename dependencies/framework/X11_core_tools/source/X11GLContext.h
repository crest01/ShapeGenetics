


#ifndef INCLUDED_PLATFORM_X11_GL_CONTEXT
#define INCLUDED_PLATFORM_X11_GL_CONTEXT

#pragma once

#include <utility>

#include "X11/platform.h"


namespace X11
{
	class GLContext
	{
	private:
		GLContext(const GLContext&);
		GLContext& operator =(const GLContext&);

		::Display* display;
		GLXContext context;

	public:
		GLContext()
			: display(nullptr),
			  context(0)
		{
		}
    
		GLContext(::Display* display, GLXContext context)
			: display(display),
			  context(context)
		{
		}

		GLContext(GLContext&& c)
			: display(c.display),
			  context(c.context)
		{
			c.context = 0;
		}

		~GLContext()
		{
			if (context)
				glXDestroyContext(display, context);
		}

		GLContext& operator =(GLContext&& c)
		{
			using std::swap;
			swap(context, c.context);
			return *this;
		}

		operator GLXContext() const { return context; }
	};

	GLContext createGLContext(::Display* display, GLXFBConfig fb_config, int major, int minor);
}

#endif  // INCLUDED_PLATFORM_X11_GL_CONTEXT

