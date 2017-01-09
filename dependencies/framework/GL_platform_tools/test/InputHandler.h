


#ifndef INCLUDED_INPUT_HANDLER
#define INCLUDED_INPUT_HANDLER

#pragma once

#include <GL/platform/InputHandler.h>


class InputHandler : public virtual GL::platform::KeyboardInputHandler, public virtual GL::platform::MouseInputHandler
{
public:
	InputHandler() = default;

	InputHandler(const InputHandler&) = delete;
	InputHandler& operator =(const InputHandler&) = delete;


	void keyDown(GL::platform::Key key) override;
	void keyUp(GL::platform::Key key) override;
	void buttonDown(GL::platform::Button button, int x, int y) override;
	void buttonUp(GL::platform::Button button, int x, int y) override;

	void mouseMove(int x, int y) override;
	void mouseWheel(int delta) override;
};

#endif  // INCLUDED_INPUT_HANDLER
