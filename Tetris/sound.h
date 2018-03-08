#pragma once
#include <windows.h>
class Sound
{

	Sound();

	void playAudio();
	void playAudio(bool replay);

	HMODULE sounds;

};