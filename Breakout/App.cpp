#include "App.h"
#include <iostream>
#include <SDL/SDL.h>

App::App()
{
}


App::~App()
{
}


void App::run()
{
	initSystems();

	while (!m_exit) {
		gameLoop();
	}
}


void App::initSystems()
{
	//// Initialize Engine ////
	Logger::Log("Initializing SDL & OpenGL...");
		Engine::init();
	Logger::Log("SDL & OpenGL initialized.");

	//// Create Window ////
	m_window = Engine::Window();
	m_window.create("Breakout", 1400, 900, false, 0);

	//// Create Shaders ////
	Logger::Log("Compiling shaders...");
		m_resources.LoadShader(std::string("Test Shader"), "Shaders/testShader.vert", "Shaders/testShader.frag");
	Logger::Log("Done compiling shaders.");

	//// Set texture uniforms ////
	Logger::Log("Setting texture uniforms...");
		m_resources.GetShader(std::string("Test Shader")).use();
		m_resources.GetShader(std::string("Test Shader")).setInt("image", 0);
		m_resources.GetShader(std::string("Test Shader")).unuse();
	Logger::Log("Texture uniforms set.");

	//// Load Textures ////
	Logger::Log("Loading textures and initializing levels...");
		m_background = Engine::Resources::LoadImage("Textures/background.jpg");
		m_levelManager.init(m_window.getWidth(), m_window.getHeight());
	Logger::Log("Finished loading textures and levels.");

	//// Initialize renderer ////
	Logger::Log("Initializing renderer...");
		m_renderer.init();
	Logger::Log("Renderer initialized.");

	//// Initialize timing ////
	Logger::Log("Initializing timing systems...");
		m_timing.init(144.0f);
	Logger::Log("Timing systems initialized.");
}


void App::gameLoop()
{
	m_timing.beginUpdate();

	processInput();
	m_inputManager.update();

	int i = 0;
	while (m_timing.getDeltaTime() > 0.0f && i++ < Engine::MAX_PHYSICS_STEPS) {
		float deltaTime = m_timing.updateDeltaTime();
		m_levelManager.update(&m_inputManager, deltaTime);
	}

	draw();

	m_timing.endUpdate();
}


void App::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_exit = true;
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
			break;
		}
	}
}


void App::draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::string name = "Test Shader";
	Engine::ShaderProgram shader = m_resources.GetShader(name);
	//m_renderer.render(
	//	m_background,
	//	&shader,
	//	0, 0,
	//	&m_window
	//);

	m_levelManager.render();

	m_window.swapBuffer();
}