#pragma once

#include "Powerups.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Engine/ShaderProgram.h>

const float SHAKE_EFFECT_LENGTH_SECONDS = 0.1f;
const float CHAOS_EFFECT_LENGTH_SECONDS = 10.0f;
const float CONFUSE_EFFECT_LENGTH_SECONDS = 10.0f;

class PostProcessor
{
public:
	PostProcessor();
	~PostProcessor();

	void init(float &screenWidth, float &screenHeight);
	void startRender();
	void endRender();
	void renderEffects();
	void startEffect(Powerup powerup);
	void update(); // Updates all effects
private:
	float m_shakeTimer = 0;
	float m_chaosTimer = 0;
	float m_confuseTimer = 0;

	void setupRendering();

	float m_windowWidth;
	float m_windowHeight;

	GLuint m_fbo;
	GLuint m_rbo;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_msfbo;
	GLuint m_screenTexture;
	Engine::ShaderProgram m_shader;
};

