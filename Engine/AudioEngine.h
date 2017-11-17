#pragma once

#include <string>
#include <irrKlang/irrKlang.h>

namespace Engine {

class AudioEngine
{
public:
	static void SetVolume(float volume);
	static void Play2D(const std::string &path, bool loop);
private:
	static irrklang::ISoundEngine *m_SoundEngine;
};

}