#include "Timing.h"
#include "Logger.h"
#include <SDL/SDL.h>
#include <algorithm>

namespace Engine {

Timing::Timing()
{
}


Timing::~Timing()
{
}


void Timing::init(float desiredFPS)
{
	m_desiredFPS = desiredFPS;
	m_previousTicks = SDL_GetTicks();
}


void Timing::beginUpdate()
{
	m_startTicks = SDL_GetTicks();
	m_deltaTime = m_frameTime / DESIRED_FRAMETIME;
}


void Timing::endUpdate()
{	
	calculateFPS();

	// Limit FPS if it's higher than the desired amount
	float frameTicks = SDL_GetTicks() - m_startTicks;
	if (m_desiredFPS > 0.0f) {
		if (1000.0f / m_desiredFPS > frameTicks) {
			SDL_Delay(1000.0f / m_desiredFPS - frameTicks);
		}
	}

	/*
	if (m_currentFrame++ == 10) {
		Logger::Log("FPS: " + std::to_string(m_fps));
		m_currentFrame = 0;
	}
	*/
}


float Timing::updateDeltaTime()
{
	// Update delta time
	float deltaTime = std::min(m_deltaTime, MAX_DELTA_TIME);
	m_deltaTime -= deltaTime;
	return deltaTime;
}


void Timing::calculateFPS()
{
	static int currFrame = 0;
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	int index = currFrame % NUM_SAMPLES;

	m_currentTicks = SDL_GetTicks();

	m_frameTime = m_currentTicks - m_previousTicks;
	frameTimes[index] = m_frameTime;

	m_previousTicks = m_currentTicks;

	int count = (currFrame++ < NUM_SAMPLES) ? currFrame : NUM_SAMPLES;

	float averageFrameTime = 0.0f;
	for (size_t i = 0; i < count; i++) {
		averageFrameTime += frameTimes[i];
	}
	averageFrameTime /= static_cast<float>(count);

	if (averageFrameTime > 0.0f) {
		m_fps = 1000.0f / averageFrameTime;
	}
	else {
		m_fps = 3.141592;
	}

	if (currFrame == NUM_SAMPLES) currFrame = 0;
}

}