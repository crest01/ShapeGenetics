


#include "Navigator.h"
#include "InstancedRenderer.h"

#include "ConsoleHandler.h"


ConsoleHandler::ConsoleHandler(Navigator& navigator, InstancedRenderer& rendering_system)
	: navigator(navigator),
	  renderer(rendering_system)
{
}

void ConsoleHandler::command(const char* command, size_t length)
{

}
