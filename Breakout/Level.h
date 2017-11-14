#pragma once

#include "Object.h"
#include "PostProcessor.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <Engine/ShaderProgram.h>
#include <Engine/GLTexture2D.h>

class Level
{
public:
	Level();
	~Level();

	void init(const std::string& path, float windowWidth, float windowHeight);
	void update(PostProcessor* postprocessor, float deltaTime);
	void breakBlock(int idx);
	void render(Engine::ShaderProgram *shader, Engine::GLTexture2D &paddle, Engine::GLTexture2D &ball, Engine::GLTexture2D &block, Engine::GLTexture2D solidBlock, Engine::GLTexture2D &background);
	void movePaddle(float xChange, float yChange, float deltaTime);
	void moveBall(float xChange, float yChange, float deltaTime);
	void setBallMovement(const glm::vec2 &dir, float speed);
	void setBallMovement(const glm::vec2 &dir);
	void clampStickyBall();
	Object getPaddle() { return m_paddle; }
private:
	void setupRendering();
	bool checkCollision(PostProcessor* postProcessor);

	GLuint m_vao;
	GLuint m_vbo;
	Object m_ball;
	Object m_paddle;
	float m_windowWidth;
	float m_windowHeight;
	std::vector<Object> m_blocks;
};

