


#include <iostream>
#include <stdexcept>

#include <GL/platform/Application.h>

#include "Config.h"
#include "OrbitalNavigator.h"
#include "FirstPersonNavigator.h"
#include "ConsoleHandler.h"
#include "InputHandler.h"

#include "InstancedRenderer.h"
#include "GeneticAlgoScene.h"

void compiletest();


int main(int argc, char* argv[])
{
	compiletest();
	try
	{

		std::cout << "usage: procrenderer configfile.cfg [-batchmode]" << std::endl;
		std::cout << "Keys: " << std::endl;
		std::cout << "   r ... start Genetic Algorithm" << std::endl;
		std::cout << "   s ... save camera position" << std::endl;
		std::cout << "   n ... show next individual" << std::endl;
		std::cout << "   p ... show previous individual" << std::endl;
		std::cout << "   v ... show voxels (only for Volume Evaluation on the GPU)" << std::endl;
		std::cout << "   t ... show target (only for Volume Evaluation)" << std::endl;
		std::cout << "   c ... set camera to evaluation pos (only for image Evaluation)" << std::endl;
		std::cout << "   q ... quit and save camera Position" << std::endl;

		Config config = loadConfig("camera_config.cfg");
		//ExperimentationScene scene;

		std::string config_file = "ga_config.cfg";
		if (argc == 2) {
			config_file = argv[1];
		}

		bool autostart = false;
		if (argc == 3) {
			config_file = argv[1];
			std::string tmp("-batchmode");
			autostart = (tmp.compare(argv[2]) == 0);
		}


		GeneticAlgoScene scene(config_file);

		//Renderer renderer(config, scene);
		InstancedRenderer renderer(config, scene);

		renderer.setBatchMode(autostart);

		OrbitalNavigator navigator(config.loadConfig("orbital_navigator"), -math::constants<float>::pi() * 0.5f, 0.0f, 10.0f);
		//FirstPersonNavigator navigator(20.0f, 1.0f, math::constants<float>::pi() * 0.5f, 0.0f);

		InputHandler input_handler(navigator, renderer, scene);

		renderer.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		renderer.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));
		renderer.attach(&navigator);

		scene.init();

		GL::platform::run(renderer);

		navigator.save(config.loadConfig("orbital_navigator"));
		renderer.save(config);

		save(config, "camera_config.cfg");
	}
	catch (std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return 0;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		return 0;
	}

	return 0;
}
