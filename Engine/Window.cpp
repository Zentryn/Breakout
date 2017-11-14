#include "Window.h"
#include "Logger.h"

namespace Engine {

void Window::create(const char *title, const GLuint width, const GLuint height, const bool vSync, const Uint32 flags)
{
	m_width = width;
	m_height = height;

	Uint32 currentFlags = SDL_WINDOW_OPENGL;

	if (flags & WindowFlags::INVISIBLE) {
		currentFlags |= SDL_WINDOW_HIDDEN;
		m_flags.push_back(WindowFlags::INVISIBLE);
	}

	if (flags & WindowFlags::BORDERLESS) {
		currentFlags |= SDL_WINDOW_BORDERLESS;
		m_flags.push_back(WindowFlags::BORDERLESS);
	}

	if (flags & WindowFlags::FULLSCREEN) {
		currentFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		m_flags.push_back(WindowFlags::FULLSCREEN);
	}

	//// Create window ////
	Logger::Log("Creating main window...");
		m_sdlWindow = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			currentFlags
		);
		
		if (m_sdlWindow == nullptr) {
			Logger::Error("SDL window could not be created!");
		}
	Logger::Log("Main window created!");

	//// Set up SDL GL context ////
	SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
	if (glContext == nullptr) {
		Logger::Error("SDL GL Context could not be created!");
	}

	//// Initialize glew ////
	GLenum glewStatus = glewInit();

	if (glewStatus != GLEW_OK) {
		Logger::Error("Glew could not be initialized!");
	}

	//// Print the current version of OpenGL ////
	Logger::Logf("OpenGL Version is %s", glGetString(GL_VERSION));

	//// Set window's background color ////
	glClearColor(0.2f, 0.6f, 0.1f, 1.0f);

	//// Set vertical sync ////
	SDL_GL_SetSwapInterval(static_cast<int>(vSync));
	(vSync) ? Logger::Log("VSync is on.") : Logger::Log("VSync is off.");

	//// Enable alpha blending ////
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Make cursor invisible and make it stay in the center ////
	//SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
}


void Window::swapBuffer()
{
	SDL_GL_SwapWindow(m_sdlWindow);
}

}
