#pragma once

#include "Object.h"
#include "PostProcessor.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <Engine/ShaderProgram.h>
#include <Engine/GLTexture2D.h>

const float BREAK_THROUGH_LENGTH = 3.0f;

class Level
{
public:
	Level();
	~Level();

	void init(const std::string& path, float windowWidth, float windowHeight);
	void update(PostProcessor* postprocessor, float deltaTime);
	void breakBlock(int idx, PostProcessor* postprocessor);
	void render(Engine::ShaderProgram *shader, Engine::GLTexture2D &paddle, Engine::GLTexture2D &ball, Engine::GLTexture2D &block, Engine::GLTexture2D solidBlock, Engine::GLTexture2D &background, Engine::GLTexture2D &powerupSpeed, Engine::GLTexture2D &powerupSticky, Engine::GLTexture2D &powerupPassthrough, Engine::GLTexture2D &powerupWidth, Engine::GLTexture2D &powerupChaos, Engine::GLTexture2D &powerupConfuse);
	void movePaddle(float xChange, float yChange, float deltaTime);
	void moveBall(float xChange, float yChange, float deltaTime);
	void setBallMovement(const glm::vec2 &dir, float speed);
	void setBallMovement(const glm::vec2 &dir);
	void clampStickyBall();
	Object getPaddle() { return m_paddle; }
private:
	void spawnPowerup(Object* block, PostProcessor* postprocessor);
	void setupRendering();
	void resetBall();
	void applyPowerup(Powerup powerup);
	bool checkCollision(PostProcessor* postProcessor);

	GLuint m_vao;
	GLuint m_vbo;
	Object m_ball;
	Object m_paddle;
	float m_windowWidth;
	float m_windowHeight;
	float m_storedBallSpeed = 0.0f;
	bool m_stickToPaddle = false;
	float m_breakThroughTimer = 0.0f;
	std::vector<Object> m_blocks;
	std::vector<Object> m_powerupObjects;
};

