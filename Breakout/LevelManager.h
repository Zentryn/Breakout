#pragma once

#include "Level.h"
#include "PostProcessor.h"
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Engine/GLTexture2D.h>
#include <Engine/ShaderProgram.h>
#include <Engine/InputManager.h>

struct Powerups {
	bool stickyPaddle = false;
};

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	void init(float windowWidth, float windowHeight);
	void render();

	void update(Engine::InputManager *inputManager, float &deltaTime);

private:
	PostProcessor m_postProcessor;
	Powerups m_powerups;
	float m_windowWidth;
	float m_windowHeight;
	int m_currentLevel = 0;
	std::map<int, Level> m_levels;
	Engine::ShaderProgram m_shader;
	Engine::ShaderProgram m_effectShader;

	GLuint m_screenTexture;

	Engine::GLTexture2D m_background;
	Engine::GLTexture2D m_paddle;
	Engine::GLTexture2D m_ball;
	Engine::GLTexture2D m_block;
	Engine::GLTexture2D m_solidBlock;
};

