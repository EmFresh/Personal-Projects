#include "Sound.h"
using namespace std;

Sound::Sound(std::string sound) :sound(sound)
{
	sndPlaySoundA(sound.c_str(), SND_ASYNC);
	sndPlaySoundA(NULL, NULL);
}
Sound::Sound(const char* sound) : sound(sound)
{
	sndPlaySoundA(sound, SND_ASYNC);
	sndPlaySound(NULL, NULL);
}

void Sound::play(bool loop)
{
	sndPlaySoundA(NULL, SND_SYNC);
	sndPlaySoundA(sound.c_str(), SND_ASYNC | (loop ? SND_LOOP : NULL));
}
void Sound::play()
{
	play(false);
}

void Sound::stopSound()
{
	sndPlaySound(NULL, NULL);
}

