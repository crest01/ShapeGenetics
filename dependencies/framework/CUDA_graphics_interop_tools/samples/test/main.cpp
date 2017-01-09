


#include <exception>
#include <iostream>

#include <GL/platform/Application.h>

#include <GL/error.h>

#include <cuda.h>
#include <CUDA/error.h>

#include "Display.h"
#include "InputHandler.h"
#include "Renderer.h"


int main()
{
	try
	{
		succeed(cuInit(0U));

		Display display;
		InputHandler input_handler;
		Renderer renderer;

		display.attach(&renderer);
		display.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		display.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));

		GL::platform::run(display);
	}
	catch (const GL::error& e)
	{
		std::cout << e.name() << ": " << e.what() << std::endl;
		return -1;
	}
	catch (const CU::error& e)
	{
		std::cout << e.name() << ": " << e.what() << std::endl;
		return -1;
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown error\n";
		return -128;
	}

	return 0;
}
