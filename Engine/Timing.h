#pragma once

namespace Engine {

const int MAX_PHYSICS_STEPS = 6;
const float DESIRED_FPS = 60.0f;
const float MAX_DELTA_TIME = 1.0f;
const float MS_PER_SECOND = 1000.0f;
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;

class Timing
{
public:
	Timing();
	~Timing();

	void init(float desiredFPS = DESIRED_FPS);
	void beginUpdate();
	void endUpdate();
	float updateDeltaTime();

	float getStartTicks() { return m_startTicks; }
	float getFrameTime() { return m_frameTime; }
	float getDeltaTime() { return m_deltaTime; }

private:
	void calculateFPS();

	float m_deltaTime;
	float m_startTicks;     /// Ticks at the start of a frame
	float m_frameTime;      /// MS taken to render a frame
	float m_desiredFPS;     /// The wanted amount of FPS
	float m_fps;            /// Actual amount of FPS
	int m_currentFrame = 0; /// Frame counter used for logging
	float m_currentTicks;   
	float m_previousTicks;  /// MS taken to render the last frame
};

}