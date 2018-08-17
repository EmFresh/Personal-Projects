﻿#include "EmConsole.h"
#include "keyinput.h"
#include "AudioPlayer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>

using namespace std;
namespace fs = filesystem;

typedef unsigned short ushort;

#pragma region Global Variables


EmConsole *con = new EmConsole("RockBand Console");

SpriteSheet *track = new SpriteSheet, *notes = new SpriteSheet, *logo = new SpriteSheet;
Sprite *box = new Sprite;
vector<vector<int>>*disiNotes = new vector<vector<int>>(5, vector<int>());
vector<int>*fretColour = new vector<int>{FG_GREEN,FG_RED,FG_YELLOW,
FG_BLUE,FG_PURPLE}, *noteColour = new vector<int>{FG_GREEN | FG_INTENSIFY,FG_RED | FG_INTENSIFY,
FG_YELLOW | FG_INTENSIFY,FG_BLUE | FG_INTENSIFY,FG_PURPLE | FG_INTENSIFY};
vector <int> *lines = new vector <int>;

vector<vector<long>>*song = new vector<vector<long>>(5), *songTemp = new vector<vector<long>>(5);
long *lastNote = new long, *incrimentCounter = new long;
int trackSpeed[5], colliCount[5], barCount, centerTrack, countAmount[5], notesHit, noteOffset = 0, fretOffset, fretboardPosition, speed = 60, create;
float  fps = 0;
long incriment = -1;
bool pressed[5], stroke, start = true;
KeyInput key;

int vol, mute, res, songChoice, songChoiceCounter;
string* songName = new string();
wstring *percentstr = new wstring;
clock_t* tsT = new clock_t, *diffT = new clock_t, *startT = new clock_t, *endT = new clock_t;
thread *t = new thread, *t2 = new thread;
AudioPlayer* sound = new AudioPlayer();//.wav files can only be 12 characters long
#pragma endregion

#pragma region Deffinitions
void saveSong();
void openSong(string&);
bool collision();
void noteDelete();
wstring cDir(wstring);
void reset();
void missFX(int seed);
void playButtonPress();
void drawLines();
void barLines();
bool invisable(int, int);
void playSongMovement();
float notesInSong(vector<vector<long>>*);
void percent();
void playTrack();
void createButtonPress();
void createSongMovement();
void createTrack();
bool startScreen();
void songChoiceMovement(int);
bool createdSongList();
bool songList();
void calculateFPS();
void controles();
#pragma endregion

#pragma region File I/O

wstring cDir(wstring s)
{
	for(auto &a : s)
		if(a == '\\')
			a = '/';
	return s;
}
void saveSong()
{
	string name("songs/" + songName->substr(2, songName->find_last_of('.') - 2) + ".song");
	fstream songs(name, ios::out | ios::binary);
	int count = 0;
	for(auto &a : *song)
	{
		//	OutputDebugString((to_string(0)+"\n").c_str());
		songs << a.size() << endl;
		for(auto &b : a)
			songs << b;
		songs << endl;
	}
	songs << *songName << endl;
	songs.close();
}
void openSong(string &songFile)
{
	string name("songs/" + songFile + ".song");
	fstream songs(name, ios::in | ios::binary);
	int c;
	for(auto &a : *song)
	{
		songs >> c;
		a.clear();
		//OutputDebugString((to_string(a.size())+"\n").c_str());
		for(int b = 0; b < c; b++)
		{
			a.push_back(0);
			songs >> a[b];
			//	OutputDebugString((to_string(a[b]) + "\n").c_str());
		}
	}
	getline(songs >> ws, *songName);

	songs.close();

	OutputDebugStringA(("Now Playing: " + *songName + '\n').c_str());

}
#pragma endregion	

#pragma region Create

void createButtonPress()
{
	incriment = sound->getPosition();

	//Key Stroke
	if(key.stroke(VK_RETURN))
		saveSong();
	if(key.stroke('R'))
	{
		for(auto &a : *song)
			a.clear();
		reset();
	}

	static char frets[] = {'A','S','D','F','G'};

	//Key Pressed
	//strum
	if((key.press(VK_UP) || key.press(VK_DOWN)) && !stroke)
	{
		stroke = true;
		for(short a = 0; a < 5; a++)
			if(key.press(frets[a]))
				((*song)[a]).push_back(-incriment);
	}

	//fret light
	for(short a = 0; a < 5; a++)
		if(key.press(frets[a]))
		{
			pressed[a] = true;
			if((*fretColour)[a] < 8)
				(*fretColour)[a] += 8;
		}


	//Key Released
	//strum
	if((key.release(VK_UP) && key.release(VK_DOWN)))
		stroke = false;

	//fret light
	for(short a = 0; a < 5; a++)
		if(key.release(frets[a]))
		{
			pressed[a] = false;
			if((*fretColour)[a] > 8)
				(*fretColour)[a] -= 8;
		}

}

void createTrack()
{
	con->toConsoleBuffer((*track)[0], centerTrack, 0);
	barLines();
	createButtonPress();

	// draws frets that lightup when key is pressed
	for(short a = 0; a < 5; a++)
		con->toConsoleBuffer((*notes)[0], (centerTrack) +(a * 13 + 1), fretboardPosition, (*fretColour)[a]);
}
#pragma endregion

#pragma region Play

void percent()
{
	char p2[8];
	sprintf_s(p2, "%.2f", abs((notesInSong(song) - notesHit) / notesInSong(song) * 100 - 100));
	string tmp = p2;
	tmp += '%';
	*percentstr = wstring(tmp.begin(), tmp.end());
	return;
}

void missFX(int seed)
{
	srand(seed);
	string miss[]{"sfx/Miss_1.wav","sfx/Miss_2.wav","sfx/Miss_3.wav","sfx/Miss_4.wav"};
	sound->setAudio(miss[rand() % 4]);
	sound->play();
}

bool collision()
{
	for(int a = 0; a < songTemp->size(); a++)
	{
		if(colliCount[a] < (*songTemp)[a].size())
		{
			if((*songTemp)[a][colliCount[a]] + noteOffset > fretboardPosition + 3)
				colliCount[a]++;
		} else
			if(colliCount[a] + 1 < (*songTemp)[a].size())
				if((*songTemp)[a][colliCount[a] + 1] + noteOffset > fretboardPosition - 3)
					colliCount[a] ++;

		if(pressed[a])
			if(colliCount[a] < (*songTemp)[a].size())
				if((*songTemp)[a][colliCount[a]] + noteOffset > fretboardPosition - 3)
					return true;
	}
	return false;
}

void noteDelete()
{
	for(int a = 0; a < songTemp->size(); a++)
		if(pressed[a])
			if(colliCount[a] < (*songTemp)[a].size())
				if((*songTemp)[a][colliCount[a]] + noteOffset > fretboardPosition - 3 && (*songTemp)[a][colliCount[a]] + noteOffset <= fretboardPosition + 3)
					notesHit++,
					(*disiNotes)[a].push_back(colliCount[a]);

}

bool invisable(int fret, int check)
{
	for(int b = 0; b < (*disiNotes)[fret].size(); b++)
		if((*disiNotes)[fret][b] == check)
			return true;

	return false;
}

float notesInSong(vector<vector<long>>*song)
{
	int count(0);
	for(auto &a : *song)
		count += a.size();
	return count;
}

void drawLines()
{
	int barNum = 4;
	for(int a = 0; a < lines->size(); a++)
		if(incriment > 0)
			con->toConsoleBuffer(L"______________________________________________________________", centerTrack + 1, ((*lines)[a] + incriment / speed),
			(a + (barCount)) % barNum == 0 ? FG_WHITE | FG_INTENSIFY : FG_WHITE);

	//create new barlines
	for(; ((*lines)[lines->size() - 1] + incriment / speed) > (*notes)[0].getHeight();)
		lines->push_back((*lines)[lines->size() - 1] - (*notes)[0].getHeight() * 2);

	//deleat old barlines
	while(((*lines)[0] + incriment / speed) > con->getHeight())
	{
		lines->erase(lines->begin());
		(barCount)++;
		if(barCount > (barNum - 1))
			barCount = 0;
	}

	//	OutputDebugStringA(string(to_string(lines[lines.size()-1])+" : "+ to_string(notes[0].size()) + "\n").c_str());

}

void barLines()
{
	if(song->size() > 0)
	{
		//incriment = -round((clock() - *tsT) / double(*endT) * double(*lastNote)) - lastIncriment;
		//incriment = -round((sound->getPosition()) / double(*endT) * double(*lastNote)) - lastIncriment;
		incriment = sound->getPosition();
		drawLines();
	} else
	{
		incriment = sound->getPosition();
		drawLines();

	}
}

void playSongMovement()
{
	//OutputDebugString((to_string(songTemp->size()) + "\n").c_str());
	//incriment = sound->getPosition();


	for(short a = 0; a < songTemp->size(); a++)
	{
		for(int b = countAmount[a]; b < (*songTemp)[a].size(); b++)
		{

			//OutputDebugString((to_string(b)+", " ).c_str());


			// moves the song along
			(*songTemp)[a][b] = (*song)[a][b] / speed + incriment / speed + fretboardPosition;

			if((*songTemp)[a][b] <= -3)
				break;

			//note dropoff
			if((*songTemp)[a][b] + noteOffset > con->getHeight())
				countAmount[a]++;

			if(b > countAmount[a])
				if(((*songTemp)[a][b - 1] >= (abs((*songTemp)[a][b]) - abs((*songTemp)[a][b - 1]))))
				{


					if((*disiNotes)[a].size() > 0)
						if(b == (*disiNotes)[a][0])
							(*disiNotes)[a].erase((*disiNotes)[a].begin());
				}

			if(!invisable(a, b))
				if((*songTemp)[a][b] + noteOffset > -3 && (*songTemp)[a][b] + noteOffset < con->getHeight())
					if((*songTemp)[a][b] + noteOffset < fretboardPosition + notes[0][0].getHeight())
						con->toConsoleBuffer((*notes)[0], (centerTrack) +(a * 13 + 1), (*songTemp)[a][b] + noteOffset, (*noteColour)[a]);
					else
						con->toConsoleBuffer((*notes)[0], (centerTrack) +(a * 13 + 1), (*songTemp)[a][b] + noteOffset, FG_RED | FG_INTENSIFY);
		}
		//OutputDebugString((to_string(countAmount[a]) + "\n").c_str());
	}
	//lastIncriment = incriment;
}

void playButtonPress()
{
	//Key Stroke
	if(key.stroke('R'))
		reset();

	//Key Pressed
	if((key.press(VK_UP) || key.press(VK_DOWN)) && !stroke)
	{
		if(collision())
		{
			noteDelete();
			sound->setVolume();
		} else
		{
			*t = thread(missFX, rand());
			t->detach();
			sound->setVolume(.3);
		}
		stroke = true;
	}
	static char frets[] = {'A','S','D','F','G'};
	for(short a = 0; a < 5; a++)
		if(key.press(frets[a]))
		{
			pressed[a] = true;
			if((*fretColour)[a] < 8)
				(*fretColour)[a] += 8;
		}


	//Key Released
	if((key.release(VK_UP) && key.release(VK_DOWN)))
		stroke = false;
	for(short a = 0; a < 5; a++)
		if(key.release(frets[a]))
		{
			pressed[a] = false;
			if((*fretColour)[a] > 8)
				(*fretColour)[a] -= 8;
		}

}

void playTrack()
{
	//con->mouseState();
	con->toConsoleBuffer((*track)[0], centerTrack, 0);
	*t2 = thread(percent);
	t2->detach();

	barLines();
	playSongMovement();
	playButtonPress();

	con->toConsoleBuffer(*box, con->getWidth() - (*box).getWidth(), 0);
	con->toConsoleBuffer(*percentstr, con->getWidth() - (*box).getWidth() / 2 - (percentstr->size() + 1) / 2, 2);

	// draws frets that lightup when key is pressed
	for(short a = 0; a < 5; a++)
		con->toConsoleBuffer((*notes)[0], (centerTrack) +(a * 13 + 1), fretboardPosition, (*fretColour)[a]);
}
#pragma endregion

#pragma region Song Selection

void songChoiceMovement(int size)
{
	static short delay = 50;
	if(songChoiceCounter++ == delay)
	{
		songChoiceCounter = 0;
		if(key.press(VK_DOWN))
			if(songChoice < size - 1)
				songChoice++;
			else
				songChoice = 0;
		else if(key.press(VK_UP))
			if(songChoice > 0)
				songChoice--;
			else
				songChoice = size - 1;
	}
	if(key.release(VK_DOWN) && key.release(VK_UP))
		songChoiceCounter = delay;

}

bool songList()
{
	/*File Management Test*/

	wstring path = L"./", str, p;
	vector<wstring>songs, songPath;
	//vector<string> title {"",""};
	int colours[] = {FG_WHITE,FG_GREEN};

	while(!key.stroke('A'))
	{
		int last, count = 0, startHeight = con->getHeight() / 2 - songs.size(), startWidth = con->getWidth() / 2;
		for(auto &a : fs::directory_iterator(path))
		{
			p = a.path();

			if(p.substr(p.find_last_of('.') + 1) == L"wav" || p.substr(p.find_last_of('.') + 1) == L"mp3")
			{
				songPath.push_back(p = cDir(p));
				songs.push_back(p.substr(last = (p.find_last_of('/') + 1), p.find_last_of('.') - last));

				con->toConsoleBuffer(L"Song List", startWidth - 4, startHeight - 2);
				con->toConsoleBuffer(L"----------------------------", startWidth - 14, startHeight - 1);
				str = wstring(songs[count].begin(), songs[count].end());
				con->toConsoleBuffer(str, startWidth - (str.size() / 2), startHeight + count, (songChoice == count ? colours[1] : colours[0]));
				count++;
			}
		}
		songChoiceMovement(songs.size());
		*songName = string(songPath[songChoice].begin(), songPath[songChoice].end());
		con->drawConsole();
		songs.clear();
		songPath.clear();
		if(key.stroke('S'))
		{
			*songName = "";
			return false;
		}
	}

	return true;
}

bool createdSongList()
{
	wstring path = L"./songs/", str, p;
	vector<wstring>songs, songPath;
	int colours[] = {FG_WHITE,FG_GREEN};

	while(!key.stroke('A'))
	{
		int last, count = 0, startHeight = con->getHeight() / 2 - songs.size(), startWidth = con->getWidth() / 2;
		for(auto &a : fs::directory_iterator(path))
		{
			//OutputDebugString("WHat Happened?\n");
			p = a.path();

			//OutputDebugString((to_string(p.find_last_of('.')) + "\n").c_str());
			con->toConsoleBuffer(L"Song List", startWidth - 4, startHeight - 2);
			con->toConsoleBuffer(L"----------------------------", startWidth - 14, startHeight - 1);
			if(p.substr(p.find_last_of('.') + 1) == L"song")
			{
				//	OutputDebugStringA("Entered if Statement\n");
				songPath.push_back(p = cDir(p));
				songs.push_back(p.substr(last = (p.find_last_of('/') + 1), p.find_last_of('.') - last));
				str = wstring(songs[count].begin(), songs[count].end());
				con->toConsoleBuffer(str, startWidth - (str.size() / 2), startHeight + count, (songChoice == count ? colours[1] : colours[0]));
				count++;
			}
			//OutputDebugStringA("Exited if Statement\n");
		}
		songChoiceMovement(songs.size());
		if(songChoice >= 0)
			*songName = string(songs[songChoice].begin(), songs[songChoice].end());
		con->drawConsole();
		songs.clear();
		songPath.clear();
		if(key.stroke('S'))
			return false;
	}
	return true;
}
#pragma endregion

#pragma region Other

void reset()
{

	for(auto &a : colliCount)a = 0;
	for(auto &a : countAmount)a = 0;
	for(auto &a : *disiNotes)a.clear();

	sound->setAudio(*songName);
	sound->stopAll();
	sound->play();
	*songTemp = *song;
	*incrimentCounter = 0;
	lines->clear(),
		lines->push_back(con->getHeight() - 25);
	barCount = songChoice = 0;
	incriment = -1;
	start = false;
	notesHit = 0;
	//*tsT = clock();
	*tsT = 0;
	*startT = clock();
}

void controles()
{
	Sprite controles;
	controles.create("Controles.txt");
	while(!key.stroke('A') && !key.stroke('S'))
	con->toConsoleBuffer(controles, con->getWidth()/2-controles.getWidth()/2, con->getHeight() / 2 - controles.getHeight() / 2),
	con->drawConsole();
}

bool startScreen()
{
	int x = con->getWidth() / 2 - box->getWidth() / 2, y = con->getHeight() / 2 - (2 * (box->getHeight() / 2)),
		colours[] = {FG_WHITE,FG_GREEN}, numBoxes = 3;
	bool exit = false;
	con->placeConsoleCenter();
	Sprite thing;

	while(!exit)
	{

		MouseInput mouse = con->mouseState();
		con->toConsoleBuffer(to_wstring(mouse.position.X).c_str(), 0, 0);
		if(box->boxCollision(thing, {short(x),short(y)}, mouse.position))
			con->toConsoleBuffer(L"mouse colision", 0, 0);

		for(auto &a : mouse.buttons)
			switch(a.first)
			{
			case LEFT_CLICK:
				if(a.second)
					con->toConsoleBuffer(L"Left click", 0, 0);
				break;
			case RIGHT_CLICK:
				break;
			}

		x = con->getWidth() / 2 - (*box).getWidth() / 2, y = con->getHeight() / 2 - (2 * (box->getHeight() / 2));
		if(key.stroke(VK_UP))
			create--;
		if(key.stroke(VK_DOWN))
			create++;

		if(create >= numBoxes)
			create = 0;
		if(create < 0)
			create = numBoxes-1;

		con->toConsoleBuffer((*logo)[0], con->getWidth() / 2 - (*logo)[0].getWidth() / 2, 12, FG_RED | FG_INTENSIFY);
		con->toConsoleBuffer(*box, x, y, colours[create == 0]);
		con->toConsoleBuffer(L"Single Play", x + 3, y + 2, colours[create == 0]);
		con->toConsoleBuffer(*box, x, y + 5, colours[create == 1]);
		con->toConsoleBuffer(L"Create", x + 6, y + 7, colours[create == 1]);
		con->toConsoleBuffer(*box, x, y + 10, colours[create == 2]);
		con->toConsoleBuffer(L"Controles", x + 4, y + 12, colours[create == 2]);

		con->drawConsole();
		if(key.stroke('Q'))
			return false;

		if(key.stroke('A'))
			if(create == 0)	//play song
			{
				if(exit = createdSongList())
					openSong(*songName);

			} else if(create == 1)//create	song
			{
				if(exit = songList())
					for(auto &a : *song)
						a.clear();
			} else if(create== 2)
			{
				controles();
			}
	}



	reset();
	return true;
}
#pragma endregion

void calculateFPS()
{
	static const int SAMPLE = 30;
	static short count;
	static float frameTimes[SAMPLE];

	static clock_t timer;

	frameTimes[count++] = 1.f / (float(clock() - timer) / CLOCKS_PER_SEC);
	if(count == SAMPLE)
	{
		count = 0;
		fps = 0;
		for(auto &a : frameTimes)
			fps += a;
		fps /= SAMPLE;
	}

	timer = clock();
}

void main()
{
	//con->textFileToVector("Track.txt", *track);
	//con->textFileToVector("Notes.txt", *notes);
	//con->textFileToVector("Logo.txt" ,  *logo );
	logo->create("Logo.txt");
	track->create("Track.txt");
	notes->create("Notes.txt");
	con->setConsoleSize((*track)[0].getWidth() * 2, (*track)[0].getHeight());
	con->setConsolePosition(0, 0);
	con->setResizable(true);

	std::vector<std::wstring> boxy = {
		L" _______________",
		L"|               |",
		L"|               |",
		L"|_______________|"
	};

	box->create(&boxy);


	while(true)
		if(startScreen())
		{
			fretboardPosition = con->getHeight() - 7;

			*diffT = clock();
			while(!key.stroke('Q'))
			{
				centerTrack = con->getWidth() / 2 - (*track)[0].getWidth() / 2;

				if(create)
					createTrack();
				else
					playTrack();

				#ifdef _DEBUG
				calculateFPS();
				//static int counter, c2;
				wchar_t str[20];
				swprintf_s(str, L"%.2f", fps);
				con->toConsoleBuffer((wstring(L"fps: ") + str).c_str(), 0, 1);

				//if(clock() - *diffT >= CLOCKS_PER_SEC)
				//{
				//	c2 = counter;
				//	counter = 0;
				//	*diffT = clock();
				//}
				//
				//counter++;
				#endif // _DEBUG

				con->drawConsole();
			}
			if(t2->joinable())
				t2->join();
			if(t->joinable())
				t->join();
			reset();
			sound->stopAll();
		} else
			break;
		return;
}