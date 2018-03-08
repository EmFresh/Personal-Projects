#include "AudioPlayer.h"


using namespace std;
AudioPlayer::AudioPlayer()
{}

AudioPlayer::AudioPlayer(string audio)
{

	setAudio(audio);
}

AudioPlayer::~AudioPlayer()
{}
/*
./Menu Music (Main Loop).mp3
./Menu Music (Main Loop).mp3
*/
void AudioPlayer::setAudio(string audio)
{
	audio = "\"" + audio + "\"";
	*this->audio = audio;
}

void AudioPlayer::play(bool repeat)
{
	cleanUp();
	int print = 0, inerror;
	int inst = 0;
	char status[128];

	//opens an instance of the audio file
	while(inerror = mciSendStringA(string("open " + *audio + " alias " + to_string(++inst * 10)).c_str(), NULL, 0, NULL))
		if(inst > this->inst)
		{
			printError(inerror);
			break;
		}

	//changes the time format to miliseconds
	printError(mciSendStringA(string("set " + to_string(inst * 10) + " time format ms").c_str(), 0, 0, 0));
	this->inst = inst;

	//plays the audio file
	if(repeat)
	{
		printError(mciSendStringA(string("status " + to_string((inst) * 10) + " length").c_str(), status, 128, 0));
		if(!printError(mciSendStringA(string("play " + to_string(inst * 10) + " repeat from 0 to " + to_string(atoi(status) - 50)).c_str(), NULL, NULL, NULL)))
			info();
	} else
		if(!printError(mciSendStringA(string("play " + to_string(inst * 10)).c_str(), NULL, NULL, NULL)))
			info();
}

void AudioPlayer::mute()
{
	char status[128];

	printError(mciSendStringA(string("set " + to_string(inst * 10) + " audio all on").c_str(), status, 128, 0));

	if(status != "on")
		printError(mciSendStringA(string("set " + to_string(inst * 10) + " audio all on").c_str(), NULL, NULL, 0));
	else
		printError(mciSendStringA(string("set " + to_string(inst * 10) + " audio all off").c_str(), NULL, NULL, 0));
}

void AudioPlayer::pause()
{
	mciSendStringA(string("pause " + to_string(inst * 10)).c_str(), NULL, 0, NULL);
}

void AudioPlayer::resume()
{
	mciSendStringA(string("resume " + to_string(inst * 10)).c_str(), NULL, 0, NULL);
}

void AudioPlayer::stop()
{
	mciSendStringA(string("stop " + to_string(inst * 10)).c_str(), NULL, 0, NULL);
	cleanUp();
}

bool AudioPlayer::isPlaying()
{
	char info[128];

	if(printError(mciSendStringA(string("status " + to_string(inst * 10) + " mode").c_str(), info, 125, NULL)))
		return 0;

	return string(info) == "playing";
}

void AudioPlayer::info()
{
	char info[128];
	mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
	string size = string(info);
	OutputDebugStringA((string(info) + "\n").c_str());

	for(int a = 1; a <= stoi(size); a++)
	{
		mciSendStringA(string("sysinfo all name " + to_string(a) + " open").c_str(), info, 128, NULL);
		OutputDebugStringA(((string)info + " ").c_str());
		mciSendStringA(string("status " + string(info) + " mode").c_str(), info, 125, NULL);
		OutputDebugStringA(((string)info + "\n").c_str());
	}
	OutputDebugStringA("\n");
}

void AudioPlayer::setVolume(int vol)
{
	if(vol <= 1000 && vol >= 0)
		printError(mciSendStringA(string("setaudio " + to_string(inst * 10) + " volume to " + to_string(vol)).c_str(), 0, 0, 0));
}

bool AudioPlayer::printError(int error)
{
	char prob[128];
	mciGetErrorStringA(error, prob, 128);
	if(error)
	{
		OutputDebugStringA(((string)prob + "\n").c_str());
		return true;
	}
	return false;
}

void AudioPlayer::cleanUp()
{
	char info[128], error[128];
	mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
	string size = string(info);
	int deleat = 0;
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

//string AudioPlayer::openDevices()
//{
//	char info[128];
//	mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
//
//	string size = string(info);
//	OutputDebugStringA(string("Open devices " + size + "\n").c_str());
//	return size;
//}


//void AudioPlayer::replay(int inst)
//{
//	OutputDebugStringA(string("New Thread " + to_string((inst)) + "\n").c_str());
//	char info[128];
//	string size = openDevices();
//	//OutputDebugStringA(string("Open devices " + size + "\n").c_str());
//
//	printError(mciSendStringA(string("sysinfo all name " + to_string(inst) + " open").c_str(), info, 128, NULL));
//	char status[128];
//
//	while(!printError(mciSendStringA(string("status " + string(info) + " mode").c_str(), status, 128, NULL)))
//	{
//		if(printError(mciSendStringA(string("status " + to_string((inst) * 10) + " length").c_str(), status, 128, 0)))
//			OutputDebugStringA((string(status) + "\n").c_str());
//	}
//	//t.join();
//	OutputDebugStringA("The End\n");
//	//ap.t.join();
//}

//void AudioPlayer::trueReplay(int inst)
//{
//	OutputDebugStringA(string("New Thread " + to_string((inst)) + "\n").c_str());
//	char info[128];
//	mciSendStringA(string("sysinfo all quantity open").c_str(), info, 128, NULL);
//
//	string size = string(info);
//	OutputDebugStringA(string("Open devices " + size + "\n").c_str());
//
//	printError(mciSendStringA(string("sysinfo all name " + to_string(inst) + " open").c_str(), info, 128, NULL));
//	char status[128];
//
//	while(!printError(mciSendStringA(string("status " + string(info) + " mode").c_str(), status, 128, NULL)))
//	{
//		if(printError(mciSendStringA(string("status " + to_string((inst) * 10) + " length").c_str(), status, 128, 0)))
//			OutputDebugStringA((string(status) + "\n").c_str());
//	}
//	//t.join();
//	OutputDebugStringA("The End\n");
//	//ap.t.join();
//}
