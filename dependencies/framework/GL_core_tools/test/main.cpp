


#include <exception>
#include <iostream>

#include <GL/platform/Application.h>

#include <GL/error.h>

#include "Renderer.h"
#include "InputHandler.h"


int main()
{
	try
	{
		Renderer renderer;
		InputHandler input_handler;

		renderer.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		renderer.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));

		GL::platform::run(renderer);
	}
	catch (const GL::error& e)
	{
		std::cout << e.name() << ": " << e.what() << '\n';
		return -1;
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << '\n';
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown error\n";
		return -128;
	}

	return 0;
}
