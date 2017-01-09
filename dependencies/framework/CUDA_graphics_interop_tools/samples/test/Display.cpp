


#include <cstdint>
#include <memory>

#include <GL/error.h>

#include <CUDA/gl_graphics_resource.h>

#include "Renderer.h"
#include "Display.h"


const char vs[] = R"""(
#version 450

out vec2 t;

void main()
{
	vec2 p = vec2((gl_VertexID & 0x2) * 0.5f, (gl_VertexID & 0x1));
	gl_Position = vec4(p * 4.0f - 1.0f, 0.0f, 1.0f);
	t = 2.0f * p;
}
)""";


const char fs[] = R"""(
#version 450

layout(location = 0) uniform sampler2D color_buffer;

in vec2 t;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(color_buffer, t);
}
)""";


Display::Display()
	: window("test", 800, 600),
	  context(window.createContext(4, 5)),
	  ctx(context, window),
	  vs(GL::compileVertexShader(::vs)),
	  fs(GL::compileFragmentShader(::fs)),
	  renderer(nullptr)
{
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	GL::linkProgram(prog);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	window.attach(this);
	GL::throw_error();
}

void Display::resize(int width, int height)
{
	buffer_width = width;
	buffer_height = height;

	color_buffer = GL::createTexture2D(buffer_width, buffer_height, 1, GL_RGBA8);

	if (renderer)
		color_buffer_resource = CU::graphics::registerGLImage(color_buffer, GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD | CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST);
}

void Display::render()
{
	if (renderer)
	{
		CUgraphicsResource resources[] = { color_buffer_resource };
		CU::graphics::map_resources map(resources);

		auto color_buffer = CU::graphics::getMappedArray(color_buffer_resource, 0U, 0U);

		renderer->render(color_buffer, buffer_width, buffer_height);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, buffer_width, buffer_height);

	glBindVertexArray(vao);
	glUseProgram(prog);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	glBindSampler(0, sampler);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	GL::throw_error();

	ctx.swapBuffers();
}

void Display::attach(::Renderer* renderer)
{
	this->renderer = renderer;
	if (renderer)
		resize(buffer_width, buffer_height);
}

void Display::attach(GL::platform::MouseInputHandler* mouse_input)
{
	window.attach(mouse_input);
}

void Display::attach(GL::platform::KeyboardInputHandler* keyboard_input)
{
	window.attach(keyboard_input);
}
