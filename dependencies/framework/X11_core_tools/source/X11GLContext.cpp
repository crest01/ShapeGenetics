


#include <cassert>
#include <stdexcept>

#include "X11GLContext.h"


namespace X11
{
	GLContext createGLContext(::Display* display, GLXFBConfig fb_config, int major, int minor)
	{
		static struct glx_ext_loader
		{
			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs;

			glx_ext_loader()
				: glXCreateContextAttribs(reinterpret_cast<decltype(glXCreateContextAttribs)>(glXGetProcAddress(reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB"))))
			{
				if (glXCreateContextAttribs == nullptr)
					throw std::runtime_error("glXCreateContextAttribsARB() not supported.");
			}
		} glx_ext;

		static const int attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, major,
			GLX_CONTEXT_MINOR_VERSION_ARB, minor,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};

		GLXContext context = glx_ext.glXCreateContextAttribs(display, fb_config, 0, True, attribs);

		if (context == 0)
			throw std::runtime_error("glXCreateContextAttribs() failed");

		return GLContext(display, context);
	}
}

