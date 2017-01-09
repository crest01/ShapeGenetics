


#include <stdexcept>
#include <iostream>

#include <GL/gl.h>

#include <GL/platform/Context.h>
#include <GL/platform/Window.h>
#include <GL/platform/Application.h>

#include "InputHandler.h"


class Renderer : public virtual GL::platform::Renderer
{
private:
	GL::platform::Window window;
	GL::platform::Context context;
	GL::platform::context_scope<GL::platform::Window> ctx;

	InputHandler input_handler;

public:
	Renderer()
		: window("test", 800, 600),
		  context(window.createContext(4, 3)),
		  ctx(context, window)
	{
		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);

		GLuint shader = glCreateShader(GL_VERTEX_SHADER);

		window.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		window.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ctx.swapBuffers();
	}
};

int main()
{
	try
	{
		Renderer renderer;

		GL::platform::run(renderer);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown error\n";
		return -128;
	}

	return 0;
}
