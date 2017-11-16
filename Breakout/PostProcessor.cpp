#include "PostProcessor.h"
#include <Engine/Logger.h>
#include <algorithm>
#include "GameManager.h"

PostProcessor::PostProcessor()
{
}


PostProcessor::~PostProcessor()
{
}


void PostProcessor::init(float &screenWidth, float &screenHeight)
{
	// Initialize frame- and renderbuffers
	glGenFramebuffers(1, &m_fbo);
	glGenFramebuffers(1, &m_msfbo);
	glGenRenderbuffers(1, &m_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);

	// Allocate storage for rbo
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, screenWidth, screenHeight);

	// Attach MS rbo to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Warning("Multisampled FBO could not be initialized!");

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glGenTextures(1, &m_screenTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glBindTexture(GL_TEXTURE_2D, m_screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Warning("Effect framebuffer could not be initialized!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_shader.compile("Shaders/effectShader.vert", "Shaders/effectShader.frag");
	m_shader.use();
	m_shader.setInt("screenTexture", 0);

	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset, offset  },  // Top Left
		{ 0.0f,    offset  },  // Top Center
		{ offset,  offset  },  // Top Right
		{ -offset, 0.0f    },  // Center Left
		{ 0.0f,    0.0f    },  // Center Center
		{ offset,  0.0f    },  // Center Right
		{ -offset, -offset },  // Bottom Left
		{ 0.0f,   -offset  },  // Bottom Center
		{ offset, -offset  }   // Bottom Right 
	};
	m_shader.setKernel("offsets", offsets);

	float blurKernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	m_shader.setFloatArray("blur_kernel", 9, blurKernel);

	int edgeKernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	m_shader.setIntArray("edge_kernel", 9, edgeKernel);


	m_shader.unuse();

	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	setupRendering();
}


void PostProcessor::startRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_msfbo);
	glClear(GL_COLOR_BUFFER_BIT);
}


void PostProcessor::endRender()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msfbo);
	glBlitFramebuffer(0, 0, m_windowWidth, m_windowHeight, 0, 0, m_windowWidth, m_windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void PostProcessor::renderEffects()
{
	m_shader.use();
	m_shader.setInt("shake", static_cast<int>(GameManager::Powerups[SHAKE]));
	m_shader.setInt("chaos", static_cast<int>(GameManager::Powerups[CHAOS]));
	m_shader.setInt("confuse", static_cast<int>(GameManager::Powerups[CONFUSE]));
	m_shader.setFloat("time", SDL_GetTicks() / 1000.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_screenTexture);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	m_shader.unuse();
}


void PostProcessor::startEffect(Powerup powerup)
{
	if (powerup != STICKY_PADDLE) GameManager::Powerups[powerup] = true;

	if (powerup == SHAKE) {
		m_shakeTimer = SHAKE_EFFECT_LENGTH_SECONDS;
	}
	else if (powerup == CHAOS) {
		m_chaosTimer = CHAOS_EFFECT_LENGTH_SECONDS;
		GameManager::Powerups[CONFUSE] = false;
	}
	else if (powerup == CONFUSE) {
		m_confuseTimer = CONFUSE_EFFECT_LENGTH_SECONDS;
		GameManager::Powerups[CHAOS] = false;
	}
}


void PostProcessor::update()
{
	float decrease = 1.0f / 144.0f;

	m_shakeTimer = std::max(0.0f, m_shakeTimer - decrease);
	m_chaosTimer = std::max(0.0f, m_chaosTimer - decrease);
	m_confuseTimer = std::max(0.0f, m_confuseTimer - decrease);

	if (m_shakeTimer <= 0.0f)
		GameManager::Powerups[SHAKE] = false;
	if (m_chaosTimer <= 0.0f)
		GameManager::Powerups[CHAOS] = false;
	if (m_confuseTimer <= 0.0f)
		GameManager::Powerups[CONFUSE] = false;
}


void PostProcessor::setupRendering()
{
	float vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}