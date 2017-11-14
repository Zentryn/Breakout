#pragma once

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <vector>

namespace Engine {

enum WindowFlags {
	INVISIBLE = 0x1,
	BORDERLESS = 0x2,
	FULLSCREEN = 0x4
};

class Window
{
public:
	void create(const char *title, const GLuint width, const GLuint height, const bool vSync, const Uint32 flags);
	void swapBuffer();
	
	GLuint getWidth()  { return m_width; }
	GLuint getHeight() { return m_height; }

private:
	GLuint m_width;
	GLuint m_height;
	std::vector<Uint32> m_flags;
	SDL_Window* m_sdlWindow;
};

}