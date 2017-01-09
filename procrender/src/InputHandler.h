


#ifndef INCLUDED_INPUT_HANDLER
#define INCLUDED_INPUT_HANDLER

#pragma once

#include <GL/platform/InputHandler.h>
#include "RendererInput_IF.h"


class Navigator;
class Renderer;
class Scene;

class InputHandler : public virtual GL::platform::KeyboardInputHandler, public virtual GL::platform::MouseInputHandler
{
private:
	Navigator& navigator;
	RendererInput_IF& renderer;
	Scene& scene;

public:
	InputHandler(Navigator& navigator, RendererInput_IF& renderer, Scene& scene);

	InputHandler(const InputHandler&) = delete;
	InputHandler& operator =(const InputHandler&) = delete;

	void keyDown(GL::platform::Key key);
	void keyUp(GL::platform::Key key);
	void buttonDown(GL::platform::Button button, int x, int y);
	void buttonUp(GL::platform::Button button, int x, int y);
	void mouseMove(int x, int y);
	void mouseWheel(int delta);
};

#endif  // INCLUDED_INPUT_HANDLER
