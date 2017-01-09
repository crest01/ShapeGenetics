


#include <sstream>
#include <iomanip>
#include <string>

#include <GL/platform/Application.h>

#include <GL/error.h>

#include "Config.h"
#include "Display.h"


extern const char fullscreen_triangle_vs[];
extern const char draw_color_buffer_fs[];

namespace
{

#ifdef WIN32
	void APIENTRY OpenGLDebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		OutputDebugStringA(message);
		OutputDebugStringA("\n");
	}

	WINDOWPLACEMENT loadWindowPlacement(const Config& config)
	{
		WINDOWPLACEMENT window_placement;
		window_placement.length = sizeof(WINDOWPLACEMENT);
		window_placement.flags = config.loadInt("flags", 0U);
		window_placement.showCmd = config.loadInt("show", SW_SHOWNORMAL);
		window_placement.ptMinPosition.x = config.loadInt("min_x", 0);
		window_placement.ptMinPosition.y = config.loadInt("min_y", 0);
		window_placement.ptMaxPosition.x = config.loadInt("max_x", 0);
		window_placement.ptMaxPosition.y = config.loadInt("max_y", 0);
		window_placement.rcNormalPosition.left = config.loadInt("left", 0);
		window_placement.rcNormalPosition.top = config.loadInt("top", 0);
		window_placement.rcNormalPosition.right = config.loadInt("right", 800);
		window_placement.rcNormalPosition.bottom = config.loadInt("bottom", 600);
		return window_placement;
	}

	void saveWindowPlacement(Config& config, const WINDOWPLACEMENT& window_placement)
	{
		config.saveInt("flags", window_placement.flags);
		config.saveInt("show", window_placement.showCmd);
		config.saveInt("min_x", window_placement.ptMinPosition.x);
		config.saveInt("min_y", window_placement.ptMinPosition.y);
		config.saveInt("max_x", window_placement.ptMaxPosition.x);
		config.saveInt("max_y", window_placement.ptMaxPosition.y);
		config.saveInt("left", window_placement.rcNormalPosition.left);
		config.saveInt("top", window_placement.rcNormalPosition.top);
		config.saveInt("right", window_placement.rcNormalPosition.right);
		config.saveInt("bottom", window_placement.rcNormalPosition.bottom);
	}
#else

typedef int WINDOWPLACEMENT;

	void OpenGLDebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		fprintf(stderr, "%s\n", message);
	}

	WINDOWPLACEMENT loadWindowPlacement(const Config& config)
	{
		return 0;
	}

	void saveWindowPlacement(Config& config, const WINDOWPLACEMENT& window_placement)
	{
	}


#endif
}

myDisplay::myDisplay(const Config& config)
#if __WIN32__
: window("ProcGeometry", loadWindowPlacement(config.loadConfig("window_placement")), 24, 0, false),
#else
:	window("ProcGeometry", 800, 600, 24, 0, false),
 #endif
  context(window.createContext(4, 3, false)),
	  ctx(context, window),
	  buffer_width(0), buffer_height(0)
{

	window.attach(this);

	glDebugMessageCallback(OpenGLDebugOutputCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	ctx.setSwapInterval(0);
}



void myDisplay::close()
{
	GL::platform::quit();
}

void myDisplay::resize(int width, int height)
{
	buffer_width = width;
	buffer_height = height;
	setViewport();
}

void myDisplay::move(int x, int y)
{

}

void myDisplay::setViewport() const
{
	glViewportIndexedf(0U, 0.0f, 0.0f, static_cast<float>(buffer_width), static_cast<float>(buffer_height));
}

float myDisplay::getAspect() const
{
	return static_cast<float>(buffer_width) / static_cast<float>(buffer_height);
}
void myDisplay::swapBuffers()
{
	ctx.swapBuffers();
}

void myDisplay::attach(GL::platform::MouseInputHandler* mouse_input)
{
	window.attach(mouse_input);
}

void myDisplay::attach(GL::platform::KeyboardInputHandler* keyboard_input)
{
	window.attach(keyboard_input);
}

void myDisplay::save(Config& config) const
{
	WINDOWPLACEMENT placement;
#ifdef WIN32
	window.savePlacement(placement);
#endif
	saveWindowPlacement(config.loadConfig("window_placement"), placement);
}

