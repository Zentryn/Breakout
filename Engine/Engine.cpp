#include "Engine.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace Engine {
	int init()
	{
		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Enable double buffer
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}