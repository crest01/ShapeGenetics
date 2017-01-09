


#include <cstdint>
#include <memory>

#include <GL/error.h>

#include "Renderer.h"


const char vs[] = R"""(
#version 450

out vec2 t;
out vec3 color;

void main()
{
	const float s = 0.5f;
	vec2 d = vec2(-1.0f + 2.0f * (gl_VertexID & 0x1U) + 0.5f * (gl_VertexID & 0x2U), -1.0f + 1.0f * (gl_VertexID & 0x2U));
	gl_Position = vec4(s * d, 0.0f, 1.0f);

	t = d * 0.5f + 0.5f;

	uint b0 = gl_VertexID & 0x1U;
	uint b1 = (gl_VertexID & 0x2U) >> 1U;
	color = vec3((b1 | b0) == 0U ? 1.0f : 0.0f, b0 != 0U ? 1.0f : 0.0f, b1 != 0U ? 1.0f : 0.0f);
}
)""";


const char fs[] = R"""(
#version 450

layout(location = 0, binding = 0) uniform sampler2D tex;

in vec2 t;
in vec3 color;

layout(location = 0) out vec4 target;

void main()
{
	target = vec4(color, 1.0f) * texture(tex, t);
}
)""";


Renderer::Renderer()
	: window("test", 800, 600),
	  context(window.createContext(4, 5)),
	  ctx(context, window),
	  vs(GL::compileVertexShader(::vs)),
	  fs(GL::compileFragmentShader(::fs))
{
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	GL::linkProgram(prog);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	constexpr GLsizei W = 512;
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, W, W);
	{
		auto texture = std::unique_ptr<std::uint32_t[]> { new std::uint32_t[W * W] };

		constexpr unsigned int log2_S = 5;

		for (unsigned int y = 0; y < W; ++y)
			for (unsigned int x = 0; x < W; ++x)
				texture[W * y + x] = ((x >> log2_S) & 0x1U) ^ ((y >> log2_S) & 0x1U) ? 0xFFFFFFFFUL : 0x000000FFU;

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, W, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);
	}

	window.attach(this);
	GL::throw_error();
}

void Renderer::resize(int width, int height)
{
	buffer_width = width;
	buffer_height = height;
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, buffer_width, buffer_height);

	glBindVertexArray(vao);
	glUseProgram(prog);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBindSampler(0, sampler);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	GL::throw_error();

	ctx.swapBuffers();
}

void Renderer::attach(GL::platform::MouseInputHandler* mouse_input)
{
	window.attach(mouse_input);
}

void Renderer::attach(GL::platform::KeyboardInputHandler* keyboard_input)
{
	window.attach(keyboard_input);
}
