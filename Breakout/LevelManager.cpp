#include "LevelManager.h"
#include "GameManager.h"
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

	// Core textures
	m_block = Engine::Resources::LoadImage("Textures/block.png");
	m_solidBlock = Engine::Resources::LoadImage("Textures/block_solid.png");
	m_paddle = Engine::Resources::LoadImage("Textures/paddle.png");
	m_ball = Engine::Resources::LoadImage("Textures/ball.png");
	m_background = Engine::Resources::LoadImage("Textures/background.jpg");

	// Powerup textures
	m_powerupSpeed = Engine::Resources::LoadImage("Textures/powerup_speed.png");
	m_powerupSticky = Engine::Resources::LoadImage("Textures/powerup_sticky.png");
	m_powerupPassthrough = Engine::Resources::LoadImage("Textures/powerup_passthrough.png");
	m_powerupWidth = Engine::Resources::LoadImage("Textures/powerup_increase.png");
	m_powerupChaos = Engine::Resources::LoadImage("Textures/powerup_chaos.png");
	m_powerupConfuse = Engine::Resources::LoadImage("Textures/powerup_confuse.png");

	m_shader.compile("Shaders/levelShader.vert", "Shaders/levelShader.frag");

	// Load all levels from file and add them to the levels vector
	int index = 0;
	for (auto & p : fs::directory_iterator("Levels/")) {
		std::stringstream ss;
		Level level;
		ss << p;
		Logger::Log("Level found: " + ss.str());
		
		level.init(ss.str(), windowWidth, windowHeight);
		m_levels[index++] = level;
	}

	m_postProcessor.init(windowWidth, windowHeight);
}


void LevelManager::render()
{
	// First pass
	m_postProcessor.startRender();
	m_levels[m_currentLevel].render(&m_shader, m_paddle, m_ball, m_block, m_solidBlock, m_background, m_powerupSpeed, m_powerupSticky, m_powerupPassthrough, m_powerupWidth, m_powerupChaos, m_powerupConfuse);
	m_postProcessor.endRender();

	// Second pass
	m_postProcessor.renderEffects();
}


void LevelManager::update(Engine::InputManager *inputManager, float &deltaTime)
{
	m_postProcessor.update();

	int startLives = GameManager::GetLives();
	m_levels[m_currentLevel].update(&m_postProcessor, deltaTime);
	int endLives = GameManager::GetLives();

	if (startLives != endLives && !GameManager::IsGameOver()) {
		GameManager::Powerups[STICKY_PADDLE] = true;
	}

	if (inputManager->isKeyDown(SDLK_d)) {
		m_levels[m_currentLevel].movePaddle(1.0f, 0, deltaTime);
		if (GameManager::Powerups[STICKY_PADDLE]) {
			m_levels[m_currentLevel].moveBall(1.0f * m_levels[m_currentLevel].getPaddle().speed, 0, deltaTime);
			m_levels[m_currentLevel].clampStickyBall();
		}
	}
	else if (inputManager->isKeyDown(SDLK_a)) {
		m_levels[m_currentLevel].movePaddle(-1.0f, 0, deltaTime);
		if (GameManager::Powerups[STICKY_PADDLE]) {
			m_levels[m_currentLevel].moveBall(-1.0f * m_levels[m_currentLevel].getPaddle().speed, 0, deltaTime);
			m_levels[m_currentLevel].clampStickyBall();
		}
	}

	if (GameManager::IsInMenu()) {
		if (inputManager->wasKeyPressed(SDLK_UP)) {
			m_currentLevel++;
			if (m_currentLevel == m_levels.size()) m_currentLevel = 0;
		}
		if (inputManager->wasKeyPressed(SDLK_DOWN)) {
			m_currentLevel--;
			if (m_currentLevel < 0) m_currentLevel = m_levels.size() - 1;
		}
	}

	if (inputManager->wasKeyPressed(SDLK_LSHIFT)) {
		m_postProcessor.startEffect(Powerup::CHAOS);
	}
	if (inputManager->wasKeyPressed(SDLK_LCTRL)) {
		m_postProcessor.startEffect(Powerup::CONFUSE);
	}

	// Release sticky ball
	if (inputManager->wasKeyPressed(SDLK_SPACE)) {
		if (GameManager::Powerups[STICKY_PADDLE] && !GameManager::IsGameOver()) {
			GameManager::Powerups[STICKY_PADDLE] = false;
			m_levels[m_currentLevel].setBallMovement(glm::vec2(0.0f, -1.0f));
			if (GameManager::IsInMenu()) GameManager::SetInMenu(false);
		}
	}

	if (inputManager->wasKeyPressed(SDLK_ESCAPE)) {
		GameManager::SetExitGame(true);
	}
	if (inputManager->wasKeyPressed(SDLK_RETURN)) {
		if (GameManager::IsGameOver() || GameManager::IsGameWon()) {
			m_levels[m_currentLevel].reset();
			GameManager::SetGameOver(false);
			GameManager::SetGameWon(false);
		}
	}
}