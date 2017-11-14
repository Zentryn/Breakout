#include "LevelManager.h"
#include <Engine/Logger.h>
#include <Engine/Resources.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

namespace fs = std::experimental::filesystem;

LevelManager::LevelManager()
{
}


LevelManager::~LevelManager()
{
}


void LevelManager::init(float windowWidth, float windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	
	m_powerups.stickyPaddle = true;

	// Load textures
	m_block = Engine::Resources::LoadImage("Textures/block.png");
	m_solidBlock = Engine::Resources::LoadImage("Textures/block_solid.png");
	m_paddle = Engine::Resources::LoadImage("Textures/paddle.png");
	m_ball = Engine::Resources::LoadImage("Textures/ball.png");
	m_background = Engine::Resources::LoadImage("Textures/background.jpg");

	m_shader.compile("Shaders/levelShader.vert", "Shaders/levelShader.frag");

	// Load all levels from file and add them to the levels vector
	int index = 0;
	for (auto & p : fs::directory_iterator("Levels/")) {
		std::stringstream ss;
		Level level;
		ss << p;
		
		level.init(ss.str(), windowWidth, windowHeight);
		m_levels[index++] = level;
	}

	m_postProcessor.init(windowWidth, windowHeight);
}


void LevelManager::render()
{
	// First pass
	m_postProcessor.startRender();
	m_levels[m_currentLevel].render(&m_shader, m_paddle, m_ball, m_block, m_solidBlock, m_background);
	m_postProcessor.endRender();

	// Second pass
	m_postProcessor.renderEffects();
}


void LevelManager::update(Engine::InputManager *inputManager, float &deltaTime)
{
	m_postProcessor.update();
	m_levels[m_currentLevel].update(&m_postProcessor, deltaTime);

	if (inputManager->isKeyDown(SDLK_d)) {
		m_levels[m_currentLevel].movePaddle(1.0f, 0, deltaTime);
		if (m_powerups.stickyPaddle) {
			m_levels[m_currentLevel].moveBall(1.0f * m_levels[m_currentLevel].getPaddle().speed, 0, deltaTime);
			m_levels[m_currentLevel].clampStickyBall();
		}
	}
	else if (inputManager->isKeyDown(SDLK_a)) {
		m_levels[m_currentLevel].movePaddle(-1.0f, 0, deltaTime);
		if (m_powerups.stickyPaddle) {
			m_levels[m_currentLevel].moveBall(-1.0f * m_levels[m_currentLevel].getPaddle().speed, 0, deltaTime);
			m_levels[m_currentLevel].clampStickyBall();
		}
	}

	// Release sticky ball
	if (inputManager->wasKeyPressed(SDLK_SPACE)) {
		if (m_powerups.stickyPaddle) {
			m_powerups.stickyPaddle = false;
			m_levels[m_currentLevel].setBallMovement(glm::vec2(0.0f, -1.0f));
		}
	}
}