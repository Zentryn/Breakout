#include "AudioEngine.h"
#include "Logger.h"

namespace Engine {

irrklang::ISoundEngine *AudioEngine::m_SoundEngine = irrklang::createIrrKlangDevice();

void AudioEngine::SetVolume(float volume)
{
	AudioEngine::m_SoundEngine->setSoundVolume(volume);
}


void AudioEngine::Play2D(const std::string &path, bool loop)
{
	AudioEngine::m_SoundEngine->play2D(path.c_str(), loop);
	Logger::Log("Started playing this shit");
}

}