#include "AudioPlayer.h"
unsigned int AudioPlayer::_numInst, AudioPlayer::_inst;

using namespace std;
AudioPlayer::AudioPlayer()
{}

AudioPlayer::AudioPlayer(string audio)
{
	setAudio(audio);
}

void AudioPlayer::setInst(int inst)
{
	_inst = inst;
}

void AudioPlayer::setCurrentInst()
{
	_inst = _numInst;
}

AudioPlayer::~AudioPlayer()
{}

void AudioPlayer::setAudio(string audio)
{
	audio = "\"" + audio + "\"";
	*_audio = audio;
}

void AudioPlayer::play(bool repeat)
{
	cleanUp();
	int print = 0, inerror;
	int inst = 0;
	char status[128];

	//opens an instance of the _audio file
	while(inerror = mciSendStringA(string("open " + *_audio + " alias " + to_string(++inst * 10)).c_str(), NULL, 0, NULL))
		if(inst > _numInst)
		{
			printError(inerror);
			return;
		}

	//changes the time format to miliseconds
	printError(mciSendStringA(string("set " + to_string(inst * 10) + " time format ms").c_str(), 0, 0, 0));

	_inst = inst;
	_numInst = inst > _numInst ? inst : _numInst;

	//plays the _audio file
	if(repeat)
	{
		printError(mciSendStringA(string("status " + to_string((inst) * 10) + " length").c_str(), status, 128, 0));
		if(!printError(mciSendStringA(string("play " + to_string(inst * 10) + " repeat ").c_str(), NULL, NULL, NULL)))
			info();
	} else
		if(!printError(mciSendStringA(string("play " + to_string(inst * 10)).c_str(), NULL, NULL, NULL)))
			info();
}

void AudioPlayer::mute()
{
	char status[128];

	printError(mciSendStringA(string("set " + to_string(_inst * 10) + " audio all on").c_str(), status, 128, 0));

	if(status != "on") //turn mute On
		printError(mciSendStringA(string("set " + to_string(_inst * 10) + " audio all on").c_str(), NULL, NULL, 0));
	else //turn mute Off
		printError(mciSendStringA(string("set " + to_string(_inst * 10) + " audio all off").c_str(), NULL, NULL, 0));
}

void AudioPlayer::pause()
{
	mciSendStringA(string("pause " + to_string(_inst * 10)).c_str(), NULL, 0, NULL);
}

void AudioPlayer::resume()
{
	mciSendStringA(string("resume " + to_string(_inst * 10)).c_str(), NULL, 0, NULL);
}

void AudioPlayer::stop()
{
	mciSendStringA(string("stop " + to_string(_inst * 10)).c_str(), NULL, 0, NULL);
	cleanUp();//Cleans up all stopped _audio
}

void AudioPlayer::stopAll()
{
	for(int a = 0; a < _numInst; a++)
		mciSendStringA(string("stop " + to_string((a + 1) * 10)).c_str(), NULL, 0, NULL);
	cleanUp();//Cleans up all stopped _audio
}

bool AudioPlayer::isPlaying(int inst)
{
	char info[128];

	if(printError(mciSendStringA(string("status " + to_string(inst * 10) + " mode").c_str(), info, 125, NULL)))
		return 0;

	return string(info) == "playing";
}

void AudioPlayer::info()
{
	char info[128];
	mciSendStringA("sysinfo all quantity open", info, 128, NULL);
	string size = string(info), name;

	if(stoi(size)>0)
	{
		OutputDebugStringA("AudioPlayer Info:\n"/*(string(info) + "\n").c_str()*/);

		for(int a = 1; a <= stoi(size); a++)
		{
			mciSendStringA(string("sysinfo all name " + to_string(a) + " open").c_str(), info, 128, NULL);
			OutputDebugStringA((name = ((string)info + " ")).c_str());//prints instance name
			mciSendStringA(string("status " + string(info) + " mode").c_str(), info, 125, NULL);
			OutputDebugStringA(((string)info + "\n").c_str());//prints it's status (i.e. Playing, Paused...)
		}
		OutputDebugStringA("\n");
	}
}

void AudioPlayer::setVolume(float vol)
{
	if(vol <= 1 && vol >= 0)
		printError(mciSendStringA(string("setaudio " + to_string(_inst * 10) + " volume to " + to_string(int(1000 * vol))).c_str(), 0, 0, 0));
	else
		OutputDebugStringA("Invalid volume height\n");
}

bool AudioPlayer::printError(int error)
{
	char prob[128];
	mciGetErrorStringA(error, prob, 128);
	if(error)
	{
		OutputDebugStringA(("AudioPlayer Error: " + (string)prob + "\n").c_str());
		return true;
	}
	return false;
}

void AudioPlayer::cleanUp()
{
	char info[128];
	mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
	string size = string(info);

	for(int a = 1; a <= stoi(size); a++)
	{
		mciSendStringA(string("sysinfo all name " + to_string(a) + " open").c_str(), info, 128, NULL);
		mciSendStringA(string("status " + string(info) + " mode").c_str(), info, 125, NULL);

		if(string(info) == "stopped")
		{
			mciSendStringA(string("sysinfo all name " + to_string(a--) + " open").c_str(), info, 128, NULL);
			mciSendStringA(string("close " + string(info)).c_str(), 0, 0, NULL);
			OutputDebugStringA(string("close " + string(info) + "\n").c_str());
			mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
			size = string(info);
		}
	}
}
