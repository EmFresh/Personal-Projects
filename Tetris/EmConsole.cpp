#include "EmConsole.h"

using namespace std;


/***Constructers***/

Console::Console(string title)
{
	SetConsoleTitle(title.c_str());
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	//size.Top = 0;
	//size.Left = 0;
	//size.Bottom = 20;
	//size.Right = 20;
	con[0] = GetStdHandle(STD_OUTPUT_HANDLE);
	con[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	setConsoleSize(0, 0);
}

Console::Console(const char * title)
{
	SetConsoleTitle(title);
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	//size.Top = 0;
	//size.Left = 0;
	//size.Bottom = 20;
	//size.Right = 20;
	con[0] = GetStdHandle(STD_OUTPUT_HANDLE);
	con[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	setConsoleSize(0, 0);
}

Console::Console()
{
	SetConsoleTitle("New Window");
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	//size.Top = 0;
	//size.Left = 0;
	//size.Bottom = 20;
	//size.Right = 20;
	con[0] = GetStdHandle(STD_OUTPUT_HANDLE);
	con[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	setConsoleSize(0, 0);
}

/***Functions**/

/*
summary:
Creates a vector of vector string (2D vector) from a txt file

Usage:
To use this function effectively make sure your *.txt file has
no empty lines in the beginning or end of it (so no extra
vectors are not accidentaly created). Also, make sure ther is only one
space seperating every sprite from each other (for the same reason as before).
if done correctly you will basically have a sprite sheet that can be usefull for
character movement or switching items or whatever.
*/
vector<vector<string>> Console::textFileToVector(string file)
{
	fstream ascii(file, fstream::in);
	vector<vector<string>> str;

	//if(ascii.is_open())
	//	OutputDebugString("enter\n");
	//else
	//	OutputDebugString("no entry\n");

	if(ascii.is_open())
	{
		string line;
		str.push_back(vector<string>());
		for(int a = 0; getline(ascii, line);)
		{
			if(line.size() == 0)
			{
				a++;
				str.push_back(vector<string>());
			} else if(line.size() > 0)
				str[a].push_back(line);
			//OutputDebugString((line + "\n").c_str());
		}
	}

	ascii.close();
	return str;
}

//Sets title
void Console::setTitle(string title)
{
	SetConsoleTitle(title.c_str());
}

//Sets the console size
void Console::setConsoleSize(float x, float y)
{
	sz = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
	//	sz.X = x  < (sz.X) ? x : (sz.X);
	//	sz.Y = y  < (sz.Y) ? y : (sz.Y);

	SIZE.Top = 0;
	SIZE.Left = 0;
	SIZE.Bottom = y < sz.Y ? y : sz.Y - 1;
	SIZE.Right = x < sz.X ? x : sz.X - 1;

	//SIZE.Bottom = sz.Y - 1;
	//SIZE.Right = sz.X - 1;

	SetConsoleScreenBufferSize(con[1], sz);
	SetConsoleWindowInfo(con[1], true, &SIZE);

	SetConsoleScreenBufferSize(con[0], sz);
	SetConsoleWindowInfo(con[0], true, &SIZE);
}

//Sets console position
void Console::setConsolePosition(float x, float y)
{
	SetWindowPos(GetConsoleWindow(), NULL, x, y, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
	//SetWindowPos(con[0], 0, 500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//Sets the console to the center of the screen
void Console::setConsoleCenter()
{
	RECT ws, cs;
	GetWindowRect(GetDesktopWindow(), &ws);
	GetWindowRect(GetConsoleWindow(), &cs);
	SetWindowPos(GetConsoleWindow(), NULL, (ws.right / 2) - (cs.right / 2), (ws.bottom / 2) - (cs.bottom / 2), NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
}

//Sets console cursor position
void Console::consoleCursorPosition(float x, float y)
{
	cursorPosition.X = x;
	cursorPosition.Y = y;
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 1;
	SetConsoleCursorInfo(con[0], &info);
	SetConsoleCursorPosition(con[0], cursorPosition);
	//SetConsoleCursorInfo(con[1], &info);
	//SetConsoleCursorPosition(con[1], cursorPosition);
}

/*stuff to read inactive console buffer*/
vector<int> Console::readConsoleLineAtributes(float x, float y, float width)
{
	SMALL_RECT strSize;
	strSize.Top = y;
	strSize.Bottom = y + 1;
	strSize.Left = x;
	strSize.Right = x + width;

	//LPWORD a = new WORD[width];
	ci = new CHAR_INFO[width];
	ReadConsoleOutputA(con[1], ci, {(SHORT)width,1}, {(SHORT)0,(SHORT)0}, &strSize);

	vector<int>c;
	for(int d = 0; d < width; c.push_back(ci[d++].Attributes));
	

	//for(int d = 0; d < width; c.push_back(a[d++]))
	//	OutputDebugStringA((to_string(a[d])+", ").c_str());
	//OutputDebugStringA("\nend\n");
	delete[] ci;
	return c;

}

vector<vector<int>> Console::readConsoleAreaAtributes(float x, float y, float width, float height)
{
	vector<vector<int>>ans;
	for(int a = 0; a < height; a++)
		ans.push_back(readConsoleLineAtributes(x, y++, width));
//	OutputDebugStringA("\nend Area\n\n\n");
	return ans;
}

string Console::readConsoleLine(float x, float y, float width)
{
	SMALL_RECT strSize;
	strSize.Top = y;
	strSize.Bottom = y + 1;
	strSize.Left = x;
	strSize.Right = x + width;

	ci = new CHAR_INFO[width];
	ReadConsoleOutputA(con[1], ci, {(SHORT)width,1}, {(SHORT)0,(SHORT)0}, &strSize);
	
	//vector<int>e;
	//for(int d = 0; d < width; e.push_back(ci[d++].Attributes))
	//	OutputDebugStringA((to_string(ci[d].Attributes) + ", ").c_str());
//	OutputDebugStringA("\nend\n");
	string c;

	for(int a = 0; a < width; c += ci[a++].Char.UnicodeChar);
	delete[]ci;
	return c;

}

vector<string> Console::readConsoleArea(float x, float y, float width, float height)
{
	vector<string>ans;
	for(int a = 0; a < height; a++)
		ans.push_back(readConsoleLine(x, y++, width));
	return ans;
}

char Console::readConsoleCharacter(float x, float y)
{
	return readConsoleLine(x, y, 1)[0];
}

/*stuff to write console buffer*/

/*
toConsoleBuffer(string &str, float poX, float poY, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(string &str, float& poX, float& poY, float x, float y, int colour)
{
	consoleCursorPosition(0, 0);
	SMALL_RECT strSize;
	strSize.Top = y;
	strSize.Bottom = y + 1;
	strSize.Left = x;
	strSize.Right = x + 1;

	poX = poX < 0 ? 0 : poX;
	poY = poY < 0 ? 0 : poY;

	ci = new CHAR_INFO;

	for(int a = 0; a < str.size(); a++)
	{
		if(str[a] != ' ')
		{
			ci->Char.UnicodeChar = str[a];
			ci->Attributes = colour;
			WriteConsoleOutputA(con[1], ci, {1,1}, COORD {(SHORT)poX , (SHORT)poY}, &strSize);
		}
		strSize.Left = ++x;
		strSize.Right = x + 1;
	}

	delete ci;
}

/*
toConsoleBuffer(string &str, float poX, float poY, float x, float y);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(string &str, float& poX, float& poY, float x, float y)
{
	toConsoleBufferNS(str, poX, poY, x, y, FG_WHITE);
}

/*
toConsoleBuffer(string &str, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(string &str, float x, float y, int colour)
{
	float poX = 0.f, poY = 0.f;
	toConsoleBufferNS(str, poX, poY, x, y, colour);
}

/*
toConsoleBuffer(string &str, float x, float y);
* str    - string to be drawn
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds single line to the console to the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(string &str, float x, float y)
{
	toConsoleBufferNS(str, x, y, FG_WHITE);
}

/*
toConsoleBuffer(vector<string>& str, float poX, float poY, float x, float y, Colour colour);
* str    - vector of string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the specified origin (poX, poY)
to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(vector<string>& str, float& poX, float& poY, float x, float y, int colour)
{
	for(auto a : str)
		toConsoleBufferNS(a, poX, poY, x, y++, colour);
}

/*
toConsoleBuffer(vector<string>& str, float poX, float poY, float x, float y);
* str    - vector of string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(vector<string>& str, float& poX, float& poY, float x, float y)
{
	for(auto a : str)
		toConsoleBufferNS(a, poX, poY, x, y++);
}

/*
toConsoleBuffer(vector<string>& str, float x, float y, Colour colour);
* str    - vector of string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(vector<string>& str, float x, float y, int colour)
{
	for(auto a : str)
		toConsoleBufferNS(a, x, y++, colour);
}

/*
toConsoleBuffer(vector<string>& str, float x, float y);
* str    - vector of string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(vector<string>& str, float x, float y)
{
	for(auto a : str)
		toConsoleBufferNS(a, x, y++);
}

/*
toConsoleBuffer(const char* str, float poX, float poY, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(const char* str, float& poX, float& poY, float x, float y, int colour)
{
	toConsoleBufferNS(string(str), poX, poY, x, y, colour);
}

/*
toConsoleBuffer(const char* str, float poX, float poY, float x, float y);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(const char* str, float& poX, float& poY, float x, float y)
{
	toConsoleBufferNS(string(str), poX, poY, x, y, FG_WHITE);
}

/*
toConsoleBuffer(const char *str, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the specified top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBufferNS(const char *str, float x, float y, int colour)
{
	toConsoleBufferNS(string(str), x, y, colour);
}

/*
toConsoleBuffer(const char *str, float x, float y);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds single line to the console to the
specified (x,y) position from the specified top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBufferNS(const char *str, float x, float y)
{
	toConsoleBufferNS(string(str), x, y);
}

/*
toConsoleBuffer(string &str, float poX, float poY, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(string &str, float& poX, float& poY, float x, float y, vector<int> &colour)
{
	consoleCursorPosition(0, 0);

	SMALL_RECT strSize;
	strSize.Top = y;
	strSize.Bottom = y + 1;
	strSize.Left = x;
	strSize.Right = x + str.size() - 1;

	poX = poX < 0 ? 0 : poX;
	poY = poY < 0 ? 0 : poY;

	ci = new CHAR_INFO[str.size()];

	for(int a = 0; a < str.size(); a++)
	{
		ci[a].Char.UnicodeChar = str[a];
		ci[a].Attributes = colour[a];
	}

	WriteConsoleOutputA(con[1], ci, {(SHORT)str.size(),1}, COORD {(SHORT)poX , (SHORT)poY}, &strSize);
	delete[] ci;
}

/*
toConsoleBuffer(string &str, float poX, float poY, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(string &str, float& poX, float& poY, float x, float y, int colour)
{
	consoleCursorPosition(0, 0);

	SMALL_RECT strSize;
	strSize.Top = y;
	strSize.Bottom = y + 1;
	strSize.Left = x;
	strSize.Right = x + str.size() - 1;

	poX = poX < 0 ? 0 : poX;
	poY = poY < 0 ? 0 : poY;

	ci = new CHAR_INFO[str.size()];

	for(int a = 0; a < str.size(); a++)
	{
		ci[a].Char.UnicodeChar = str[a];
		ci[a].Attributes = colour;
	}

	WriteConsoleOutputA(con[1], ci, {(SHORT)str.size(),1}, COORD {(SHORT)poX , (SHORT)poY}, &strSize);
	delete[] ci;
}

/*
toConsoleBuffer(string &str, float poX, float poY, float x, float y);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(string &str, float& poX, float& poY, float x, float y)
{
	toConsoleBuffer(str, poX, poY, x, y, FG_WHITE);
}

/*
toConsoleBuffer(string &str, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(string &str, float x, float y, int colour)
{
	float poX = 0.f, poY = 0.f;
	toConsoleBuffer(str, poX, poY, x, y, colour);
}

/*
toConsoleBuffer(string &str, float x, float y);
* str    - string to be drawn
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds single line to the console to the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(string &str, float x, float y)
{
	toConsoleBuffer(str, x, y, FG_WHITE);
}

/*
toConsoleBuffer(vector<string>& str, float poX, float poY, float x, float y, Colour colour);
* str    - vector of string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the specified origin (poX, poY)
to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(vector<string>& str, float x, float y, vector<vector<int>> &colour)
{
	float p1 = 0, p2 = 0;
	for(int a = 0; a<str.size(); a++)
		toConsoleBuffer(str[a], p1, p2, x, y++, colour[a]);
}

/*
toConsoleBuffer(vector<string>& str, float poX, float poY, float x, float y, Colour colour);
* str    - vector of string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the specified origin (poX, poY)
to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(vector<string>& str, float& poX, float& poY, float x, float y, int colour)
{
	for(auto a : str)
		toConsoleBuffer(a, poX, poY, x, y++, colour);
}

/*
toConsoleBuffer(vector<string>& str, float poX, float poY, float x, float y);
* str    - vector of string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(vector<string>& str, float& poX, float& poY, float x, float y)
{
	for(auto a : str)
		toConsoleBuffer(a, poX, poY, x, y++);
}

/*
toConsoleBuffer(vector<string>& str, float x, float y, Colour colour);
* str    - vector of string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(vector<string>& str, float x, float y, int colour)
{
	for(auto a : str)
		toConsoleBuffer(a, x, y++, colour);
}

/*
toConsoleBuffer(vector<string>& str, float x, float y);
* str    - vector of string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds multiple lines to the console starting at the
specified (x,y) position from the top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(vector<string>& str, float x, float y)
{
	for(auto a : str)
		toConsoleBuffer(a, x, y++);
}

/*
toConsoleBuffer(const char* str, float poX, float poY, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(const char* str, float& poX, float& poY, float x, float y, int colour)
{
	toConsoleBuffer(string(str), poX, poY, x, y, colour);
}

/*
toConsoleBuffer(const char* str, float poX, float poY, float x, float y);
* str    - string to be drawn to buffer
* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
* x      - x position from the origin poX coordinate on the screen
* y      - y position from the origin poY coordinate on the screen

summary:
Adds single line to the console to the
specified (x,y) position from the origin (poX, poY)
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(const char* str, float& poX, float& poY, float x, float y)
{
	toConsoleBuffer(string(str), poX, poY, x, y, FG_WHITE);
}

/*
toConsoleBuffer(const char *str, float x, float y, Colour colour);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen
* colour - colour of text

summary:
Adds single line to the console to the
specified (x,y) position from the specified top left corner
to be drawn after drawConsole(); is called
(with text colour modification [see: enum colour in EmConsole.h])
*/
void Console::toConsoleBuffer(const char *str, float x, float y, int colour)
{
	toConsoleBuffer(string(str), x, y, colour);
}

/*
toConsoleBuffer(const char *str, float x, float y);
* str    - string to be drawn to buffer
* x      - x position from the top left corner of the screen
* y      - y position from the top left corner of the screen

summary:
Adds single line to the console to the
specified (x,y) position from the specified top left corner
to be drawn after drawConsole(); is called
*/
void Console::toConsoleBuffer(const char *str, float x, float y)
{
	toConsoleBuffer(string(str), x, y);
}

/*Draws the console (choose to clear console buffer after)*/
void Console::drawConsole(bool clear)
{
	setConsoleSize(SIZE.Right, SIZE.Bottom);
	if(!clear)
	{
		SetConsoleActiveScreenBuffer(con[0] = con[1]);
		counter = 1;
		clearConsole();
		counter = 0;
	} else
	{
		HANDLE temp = con[0];
		SetConsoleActiveScreenBuffer(con[0] = con[1]);
		con[1] = temp;
		counter = 1;
		clearConsole();
		counter = 0;
	}
}

/*Draws the console and clears the console buffer (clears console buffer after)*/
void Console::drawConsole()
{
	drawConsole(true);
}

/*Clears the console*/
void Console::clearConsole()
{
	DWORD cCharsWritten;
	FillConsoleOutputCharacterA(con[counter], (TCHAR)' ', sz.X*sz.Y, COORD {0,0}, &cCharsWritten);
	FillConsoleOutputAttribute(con[counter], 0, sz.X*sz.Y, COORD {0,0}, &cCharsWritten);
}