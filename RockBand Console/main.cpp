#include "EmConsole.h"
#include "keyinput.h"
#include "AudioPlayer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <thread>

using namespace std;
namespace fs = experimental::filesystem;
Console *con = new Console("RockBand Console");

vector <vector<string>> *track = new vector<vector<string>>(0), *notes = new vector<vector<string>>(0), *logo = new vector<vector<string>>(0);
vector<string> *box = new vector<string>;
vector<vector<int>>*disiNotes = new vector<vector<int>>(5, vector<int>());
vector<int>*fretColour = new vector<int> {con->FG_GREEN,con->FG_RED,con->FG_YELLOW,
con->FG_BLUE,con->FG_PURPLE}, *noteColour = new vector<int> {con->FG_GREEN | con->FG_INTENSIFY,con->FG_RED | con->FG_INTENSIFY,
con->FG_YELLOW | con->FG_INTENSIFY,con->FG_BLUE | con->FG_INTENSIFY,con->FG_PURPLE | con->FG_INTENSIFY},
*lines = new vector <int>(1, -2);

vector<vector<long>>*song = new vector<vector<long>>(5), *songTemp = new vector<vector<long>>(5);
long *lastNote = new long, *incrimentCounter = new long;
int trackSpeed[5], colliCount[5], speedCount = 1000, barCount, createSpeed,
createCountCounter = speedCount, centerTrack, countAmount[5], notesHit;
float incriment = -1, lastIncriment, createCount;
bool pressed[5], stroke, start = true, create;
keyinput key;

int vol, mute, res, songChoice, songChoiceCounter;
string* songName = new string();
string *percentstr = new string;
clock_t* tsT = new clock_t, *diffT = new clock_t, *startT = new clock_t, *endT = new clock_t;
thread *t = new thread, *t2 = new thread;
AudioPlayer* sound = new AudioPlayer();//.wav files can only be 12 characters long


#pragma region Deffinitions
void saveSong();
void openSong(string&);
bool collision();
void noteDelete();
wstring cDir(wstring );
void reset();
void missFX(int seed);
void playButtonPress();
void drawLines();
void barLines();
bool invisable(int , int );
void playSongMovement();
float notesInSong(vector<vector<long>>* );
void percent();
void playTrack();
void createButtonPress();
void createSongMovement();
void createTrack();
bool startScreen();
void songChoiceMovement(int );
bool songList();
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
	songs << songName->substr(2) << endl;
	songs << *endT - *startT-250;
	songs.close();
}
void openSong(string &songFile)
{
	string name("songs/" + songFile + ".song");
	fstream songs(name, ios::in | ios::binary);
	//song.clear();
	int c;
	//OutputDebugString("opening song\n");
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
	songs >> *endT;
	songs.close();

	*songTemp = *song;
	*lastNote = 0;
	for(auto &a : *songTemp)
		if(a.size() > 0)
			if(a[a.size() - 1] < *lastNote)
				*lastNote = a[a.size() - 1];
	OutputDebugString(("endT: " + *songName + '\n').c_str());

}
#pragma endregion	

#pragma region Create

void createButtonPress()
{
	//Key Stroke
	if(key.stroke(VK_RETURN))
		saveSong();
	if(key.stroke('R'))
	{
		for(auto &a : *song)
			a.clear();
		reset();
	}

	//Key Pressed
	static char frets[] = {'A','S','D','F','G'};
	if((key.press(VK_UP) || key.press(VK_DOWN)) && !stroke)
	{

		stroke = true;

		for(short a = 0; a < 5; a++)
			if(key.press(frets[a]))
			{
				*endT = clock();
				((*song)[a]).push_back(-(createCount)+con->getHeight() - 5);
			}
	}

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

void createSongMovement()
{
	incriment = (clock() - *tsT) / (CLOCKS_PER_SEC*.10);

	if(incriment >= 1)
		*tsT = clock();

	createCount += incriment;
	//OutputDebugStringA((to_string(createCount)+'\n').c_str());

}

void createTrack()
{
	con->toConsoleBuffer((*track)[0], centerTrack, 0);
	//barLines();
	createSongMovement();
	createButtonPress();

	// draws frets that lightup when key is pressed
	for(short a = 0; a < 5; a++)
		con->toConsoleBuffer((*notes)[0], (centerTrack)+(a * 13 + 1), con->conHeight - 5, (*fretColour)[a]);
}
#pragma endregion

#pragma region Play

void percent()
{
	char p2[8];
	sprintf_s(p2, "%.2f", abs((notesInSong(song) - notesHit) / notesInSong(song) * 100 - 100));
	*percentstr = string((char*)p2) + " %";
}

void missFX(int seed)
{
	srand(seed);
	string miss[] {"sfx/Miss_1.wav","sfx/Miss_2.wav","sfx/Miss_3.wav","sfx/Miss_4.wav"};
	sound->setAudio(miss[rand() % 4]);
	sound->play();
}

bool collision()
{
	for(int a = 0; a < songTemp->size(); a++)
	{
		if(colliCount[a] < (*songTemp)[a].size())
		{
			if((*songTemp)[a][colliCount[a]] > con->conHeight - 2)
				colliCount[a]++;
		} else if(colliCount[a] + 1 < (*songTemp)[a].size())
			if((*songTemp)[a][colliCount[a] + 1] > con->conHeight - 8)
				colliCount[a] += 2;

		if(pressed[a])
			if(colliCount[a] < (*songTemp)[a].size())
				if((*songTemp)[a][colliCount[a]] > con->conHeight - 8)
					return true;
	}
	return false;
}

void noteDelete()
{
	for(int a = 0; a < songTemp->size(); a++)
		if(pressed[a])
			if(colliCount[a] < (*songTemp)[a].size())
				if((*songTemp)[a][colliCount[a]] > con->conHeight - 8 && (*songTemp)[a][colliCount[a]] <= con->conHeight - 2)
				{
					//(*songTemp)[a].erase((*songTemp)[a].begin() + colliCount[a]);
					notesHit++;
					(*disiNotes)[a].push_back(colliCount[a]);
				}
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
	{
		if(incriment > 0)
			(*lines)[a] += incriment;
		if((a + (barCount)) % barNum == 0)
			con->toConsoleBuffer("______________________________________________________________", centerTrack + 1, (*lines)[a], con->FG_WHITE | con->FG_INTENSIFY);
		else
			con->toConsoleBuffer("______________________________________________________________", centerTrack + 1, (*lines)[a]);
	}

	for(; (*lines)[lines->size() - 1] > (float)(*notes)[0].size();)
		lines->push_back((*lines)[lines->size() - 1] - (float)(*notes)[0].size() * 2);

	while((*lines)[0] > con->getHeight())
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
		incriment = -round((clock() - *tsT) / double(*endT) * double(*lastNote)) - lastIncriment;
		lastIncriment += incriment;
		drawLines();
	} else
	{
		static clock_t in2 = 0;
		incriment = floor((((in2 = clock()) - *tsT) / (*endT / (*lastNote - con->getHeight()))));
		if(incriment > 0)
		{
			//*incrimentCounter += incriment;
			*tsT = in2 + ((in2 - *tsT) - (*endT / (*lastNote - con->getHeight())));
		}
		drawLines();

	}
}

void playSongMovement()
{
	//OutputDebugString((to_string(songTemp->size()) + "\n").c_str());
	for(short a = 0; a < songTemp->size(); a++)
	{
		for(int b = countAmount[a]; b < (*songTemp)[a].size(); b++)
		{

			//OutputDebugString((to_string(b)+", " ).c_str());

			// moves the song along
			(*songTemp)[a][b] += round(incriment);

			// checks weather or not to draw notes to screen
			if(b > countAmount[a] && (*songTemp)[a][b] <= -3)
				if((*songTemp)[a][b - 1] >= (abs((*song)[a][b]) - abs((*song)[a][b - 1])))
					(*songTemp)[a][b] = (*songTemp)[a][b - 1] - (abs((*song)[a][b]) - abs((*song)[a][b - 1]));
				else
					break;

			if(b > countAmount[a])
				if((*songTemp)[a][b] > -3 && ((*songTemp)[a][b - 1] >= (abs((*song)[a][b]) - abs((*song)[a][b - 1]))))
				{
					if((*songTemp)[a][b - 1] > con->getHeight())
						countAmount[a]++;

					if((*disiNotes)[a].size() > 0)
						if(b == (*disiNotes)[a][0])
							(*disiNotes)[a].erase((*disiNotes)[a].begin());
				}

			if(!invisable(a, b))
				if((*songTemp)[a][b] > -3 && (*songTemp)[a][b] < con->getHeight())
					if((*songTemp)[a][b] < con->getHeight() - 5)
						con->toConsoleBuffer((*notes)[0], (centerTrack)+(a * 13 + 1), (*songTemp)[a][b], (*noteColour)[a]);
					else
						con->toConsoleBuffer((*notes)[0], (centerTrack)+(a * 13 + 1), (*songTemp)[a][b], con->FG_RED | con->FG_INTENSIFY);
		}
		//OutputDebugString((to_string(countAmount[a]) + "\n").c_str());
	}
	//if(incriment > 0)
	//	*tsT = clock();
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
			sound->setVolume(300);
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
	con->toConsoleBuffer((*track)[0], centerTrack, 0);
	*t2 = thread(percent);
	t2->detach();

	barLines();
	playSongMovement();
	playButtonPress();

	con->toConsoleBuffer(*box, con->conWidth - (*box)[0].size(), 0);
	con->toConsoleBuffer(*percentstr, con->conWidth - (*box)[0].size() / 2 - (percentstr->size() + 1) / 2, 2);

	// draws frets that lightup when key is pressed
	for(short a = 0; a < 5; a++)
		con->toConsoleBuffer((*notes)[0], (centerTrack)+(a * 13 + 1), con->conHeight - 5, (*fretColour)[a]);
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

	string path = "./", str;
	wstring p;
	vector<wstring>songs, songPath;
	//vector<string> title {"",""};
	int colours[] = {con->FG_WHITE,con->FG_GREEN};

	while(!key.stroke('A'))
	{
		int last, count = 0, startHeight = con->getHeight() / 2 - songs.size(), startWidth = con->getWidth() / 2;
		for(auto &a : fs::directory_iterator(path))
		{
			p = a.path();

			if(p.substr(p.find_last_of('.') + 1) == L"wav" || p.substr(p.find_last_of('.') + 1) == L"mp3")
			{
				songPath.push_back(cDir(p));
				songs.push_back(p.substr(last = (p.find_last_of('\\') + 1), p.find_last_of('.') - last));

				con->toConsoleBuffer("Song List", startWidth - 4, startHeight - 2);
				con->toConsoleBuffer("----------------------------", startWidth - 14, startHeight - 1);
				str = string(songs[count].begin(), songs[count].end());
				con->toConsoleBuffer(str.c_str(), startWidth - (str.size() / 2), startHeight + count, (songChoice == count ? colours[1] : colours[0]));
				count++;
			}
		}
		songChoiceMovement(songs.size());
		*songName = string(songPath[songChoice].begin(), songPath[songChoice].end());
		con->drawConsole();
		songs.clear();
		songPath.clear();
		if(key.stroke('Q'))
		{
			*songName = "";
			return false;
		}
	}

	return true;
}

void createdSongList()
{
	string path = "./songs/", str;
	wstring p;
	vector<wstring>songs, songPath;
	int colours[] = {con->FG_WHITE,con->FG_GREEN};

	while(!key.stroke('A'))
	{
		int last, count = 0, startHeight = con->getHeight() / 2 - songs.size(), startWidth = con->getWidth() / 2;
		for(auto &a : fs::directory_iterator(path))
		{
			//OutputDebugString("WHat Happened?\n");
			p = a.path();

			//OutputDebugString((to_string(p.find_last_of('.')) + "\n").c_str());
			con->toConsoleBuffer("Song List", startWidth - 4, startHeight - 2);
			con->toConsoleBuffer("----------------------------", startWidth - 14, startHeight - 1);
			if(p.substr(p.find_last_of('.') + 1) == L"song")
			{
			//	OutputDebugStringA("Entered if Statement\n");
				songPath.push_back(cDir(p));
				songs.push_back(p.substr(last = (p.find_last_of('\\') + 1), p.find_last_of('.') - last));
				str = string(songs[count].begin(), songs[count].end());
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
	}
}
#pragma endregion

#pragma region Other

void reset()
{

	for(auto &a : colliCount)a = 0;
	for(auto &a : countAmount)a = 0;
	for(auto &a : *disiNotes)a.clear();

	sound->setAudio(*songName);
	sound->stop();
	sound->play();
	*songTemp = *song;
	*incrimentCounter = 0;
	lines->clear();
	lines->push_back(-2);
	barCount = createCountCounter = songChoice = 0;
	createCount = 0;
	incriment = -1;
	lastIncriment = 0;
	start = false;
	notesHit = 0;
	*startT = clock();
	*tsT = clock();
}

bool startScreen()
{
	int x = con->getWidth() / 2 - (*box)[0].size() / 2, y = con->getHeight() / 2 - (2 * (box->size() / 2));
	int colours[] = {con->FG_WHITE,con->FG_GREEN};

	//while(10);
	while(!key.stroke('A'))
	{
		x = con->getWidth() / 2 - (*box)[0].size() / 2, y = con->getHeight() / 2 - (2 * (box->size() / 2));
		if(key.stroke(VK_UP) || key.stroke(VK_DOWN))
			create = !create;
		con->toConsoleBuffer((*logo)[0], 35, 12, con->FG_RED | con->FG_INTENSIFY);
		con->toConsoleBuffer(*box, x, y, colours[!create]);
		con->toConsoleBuffer("Single Play", x + 3, y + 2, colours[!create]);
		con->toConsoleBuffer(*box, x, y + 5, colours[create]);
		con->toConsoleBuffer("Create", x + 6, y + 7, colours[create]);
		con->drawConsole();
		if(key.stroke('Q'))
			return false;
	}

	if(create == 0)
	{
		createdSongList();
		openSong(*songName);
	} else
	{
		songList();
		for(auto &a : *song)
			a.clear();
	}

	reset();
	return true;
}
#pragma endregion

void main()
{
	con->textFileToVector("Track.txt", *track);
	con->textFileToVector("Notes.txt", *notes);
	con->textFileToVector("Logo.txt", *logo);
	con->setConsoleSize((*track)[0][0].size() * 2, (*track)[0].size());
	con->setConsolePosition(0, 0);
	con->setResizable(false);

	*box =
	{
		"._______________.",
		"|               |",
		"|               |",
		"|_______________|"};


	while(true)
		if(startScreen())
		{
			//*tsT = clock();

			*diffT = clock();
			while(!key.stroke('Q'))
			{
				centerTrack = con->getWidth() / 2 - (*track)[0][0].size() / 2;

				if(create)
					createTrack();
				else
					playTrack();

#ifdef _DEBUG
				static int counter, c2;
				con->toConsoleBuffer(string("fps: " + to_string(c2)).c_str(), 0, 1);

				if(clock() - *diffT >= CLOCKS_PER_SEC)
				{
					c2 = counter;
					counter = 0;
					*diffT = clock();
				}

				counter++;
#endif // _DEBUG

				con->drawConsole();
			}
			if(t2->joinable())
				t2->join();
			if(t->joinable())
				t->join();
			reset();
			sound->stop();
		} else
			break;
		return;
}