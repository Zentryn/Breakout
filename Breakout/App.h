#pragma once

#include "GameManager.h"
#include "LevelManager.h"

#include <vector>

#include <Engine/Engine.h>
#include <Engine/Logger.h>
#include <Engine/Window.h>
#include <Engine/Timing.h>
#include <Engine/Renderer.h>
#include <Engine/Resources.h>
#include <Engine/AudioEngine.h>
#include <Engine/GLTexture2D.h>
#include <Engine/InputManager.h>
#include <Engine/RandomEngine.h>
#include <Engine/TextRenderer.h>

class App
{
public:
	App();
	~App();

	void run();
private:
	void initSystems();
	void gameLoop();
	void processInput();
	void draw();

	LevelManager m_levelManager;

	Engine::Window m_window;
	Engine::InputManager m_inputManager;
	Engine::Renderer m_renderer;
	Engine::Resources m_resources;
	Engine::Timing m_timing;

	Engine::GLTexture2D m_block;
	Engine::GLTexture2D m_solidBlock;
	Engine::GLTexture2D m_paddle;
	Engine::GLTexture2D m_background;
};

