


#ifndef INCLUDED_CONSOLE_HANDLER
#define INCLUDED_CONSOLE_HANDLER

#pragma once

#include <GL/platform/InputHandler.h>


class Navigator;
class InstancedRenderer;

class ConsoleHandler : public virtual GL::platform::ConsoleHandler
{
private:
	Navigator& navigator;
	InstancedRenderer& renderer;

public:
	ConsoleHandler(Navigator& navigator, InstancedRenderer& renderer);

	ConsoleHandler(const ConsoleHandler&) = delete;
	ConsoleHandler& operator =(const ConsoleHandler&) = delete;

	void command(const char* command, size_t length);
};

#endif  // INCLUDED_CONSOLE_HANDLER
