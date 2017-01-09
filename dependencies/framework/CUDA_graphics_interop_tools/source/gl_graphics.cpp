


#include <CUDA/error.h>

#include "gl_graphics_resource.h"


namespace CU
{
	namespace graphics
	{
		unique_resource registerGLBuffer(GLuint buffer, unsigned int flags)
		{
			CUgraphicsResource resource;
			checkError(cuGraphicsGLRegisterBuffer(&resource, buffer, flags));
			return unique_resource(resource);
		}

		unique_resource registerGLImage(GLuint image, GLenum target, unsigned int flags)
		{
			CUgraphicsResource resource;
			checkError(cuGraphicsGLRegisterImage(&resource, image, target, flags));
			return unique_resource(resource);
		}
	}
}
