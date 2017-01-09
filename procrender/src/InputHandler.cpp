


#include <cassert>
#include <stdexcept>

#include "Navigator.h"
#include "InstancedRenderer.h"
#include "Scene.h"
#include "Config.h"
#include "OrbitalNavigator.h"

#include "InputHandler.h"
#include <random>
#include <GL/platform/Application.h>

std::random_device rd;

namespace
{
	Navigator::Button translateButton(GL::platform::Button button)
	{
		switch (button)
		{
		case GL::platform::Button::LEFT:
			return Navigator::Button::LEFT;
		case GL::platform::Button::RIGHT:
			return Navigator::Button::RIGHT;
		case GL::platform::Button::MIDDLE:
			return Navigator::Button::MIDDLE;
		}
		//assert(false);
		return static_cast<Navigator::Button>(-1);
	}
}

InputHandler::InputHandler(Navigator& navigator, RendererInput_IF& renderer, Scene& scene)
	: navigator(navigator),
	  renderer(renderer),
	  scene(scene)
{

}

void InputHandler::keyDown(GL::platform::Key key)
{
}

void InputHandler::keyUp(GL::platform::Key key)
{
	switch (key)
	{

	case GL::platform::Key::F8:
		renderer.screenshot();
		break;

	case GL::platform::Key::BACKSPACE:
		navigator.reset();
		break;

	case GL::platform::Key::C_C:
		scene.setCameraToEvalPoint(&navigator);
		break;

	case GL::platform::Key::C_R:
		scene.setSeed(rd());
		scene.recombine(true);
		renderer.regenerate();
		break;

	case GL::platform::Key::C_V:
		renderer.toggleDisplayVoxels();
		renderer.regenerate();
		break;

	case GL::platform::Key::C_T:
		scene.toggleDisplayTarget();
		renderer.regenerate();
		break;

	case GL::platform::Key::C_N:
		scene.showNext();
		renderer.regenerate();
		break;

	case GL::platform::Key::C_P:
		scene.showPrev();
		renderer.regenerate();
		break;

	case GL::platform::Key::C_G:
		scene.generateOneGeneration();
		renderer.regenerate();
		break;

	case GL::platform::Key::C_Q:
		GL::platform::quit();
		break;

	case GL::platform::Key::C_S: {
		Config config = loadConfig("config.cfg");
		OrbitalNavigator* orb = static_cast<OrbitalNavigator*>(&navigator);
		orb->save(config.loadConfig("orbital_navigator"));
		save(config, "config.cfg");
	}
		break;

//	case GL::platform::Key::C_A:
//		scene.setSeed(rd());
//		renderer.toggleAlwaysRegenerate();
//		break;
	}
}

void InputHandler::buttonDown(GL::platform::Button button, int x, int y)
{
	navigator.buttonDown(translateButton(button), x, y);
}

void InputHandler::buttonUp(GL::platform::Button button, int x, int y)
{
	navigator.buttonUp(translateButton(button), x, y);
}

void InputHandler::mouseMove(int x, int y)
{
	navigator.mouseMove(x, y);
}

void InputHandler::mouseWheel(int delta)
{
	fprintf(stderr, "mouseWheel delta %d\n", delta);
	navigator.mouseWheel(delta);
}
