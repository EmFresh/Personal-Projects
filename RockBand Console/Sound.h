#pragma once
#include <windows.h>
#include <string>
class Sound
{
public:
	
	Sound(std::string sound);

	Sound(const char * sound);

	void play(bool loop);
	void play();
	void stopSound();

	std::string sound;
};