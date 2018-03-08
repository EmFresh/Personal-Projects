#pragma once
#include<Windows.h>
#include <fstream>
#include<string>
#include<vector>
//#include <io.h>
//#include <fcntl.h>

class Console
{
public:
	/*Constructers*/
	Console(std::string title);

	Console(const char* title);

	Console();

	/*Enums*/
	/*
	summary:
	These are coulers that can be used for text coulour manipulation.

	* FG_COLOUR - change the text colour
	* BG_COLOUR - Change the colour behind each character of text

	Usage:
	to use these colours in toConsoleBuffer();, use eather  FG_COLOUR or BG_COLOUR for colour manipulation.
	to combine colours or set a FG_COLOUR and BG_COLOUR at the same time, use the
	"|" operator (i.e. FG_COLOUR | BG_COLOUR ) for more combonations.
	*/
	enum Colour
	{
		FG_BLUE = FOREGROUND_BLUE,
		FG_GREEN = FOREGROUND_GREEN,
		FG_RED = FOREGROUND_RED,
		FG_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
		FG_PURPLE = FOREGROUND_BLUE | FOREGROUND_RED,
		FG_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
		FG_YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
		FG_INTENSIFY = FOREGROUND_INTENSITY,

		BG_BLUE = BACKGROUND_BLUE,
		BG_GREEN = BACKGROUND_GREEN,
		BG_RED = BACKGROUND_RED,
		BG_WHITE = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
		BG_PURPLE = BACKGROUND_BLUE | BACKGROUND_RED,
		BG_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
		BG_YELLOW = BACKGROUND_GREEN | BACKGROUND_RED,
		BG_INTENSIFY = BACKGROUND_INTENSITY
	};

	/*Functions*/

	void textFileToVector(std::string file, std::vector<std::vector<std::string>> &str);

	std::vector<std::vector<std::string>> textFileToVector(std::string file);

	void setTitle(std::string title);

	void setConsoleSize(float x, float y);

	COORD getConsoleSize();

	int getWidth();

	int getHeight();

	void setResizable(bool resz);

	void setConsolePosition(float x, float y);

	void setConsoleCenter();

	void consoleCursorPosition(float x, float y);

	std::vector<int> readConsoleLineAtributes(float x, float y, float width);

	std::vector<std::vector<int>> readConsoleAreaAtributes(float x, float y, float width, float height);

	std::vector<std::string> readConsoleArea(float x, float y, float width, float height);

	std::string readConsoleLine(float x, float y, float width);

	char readConsoleCharacter(float x, float y);

	char readActiveConsoleCharacter(float x, float y);

	void toConsoleBufferNS(std::string & str, float & poX, float & poY, float x, float y, int colour);

	void toConsoleBufferNS(std::string & str, float & poX, float & poY, float x, float y);

	void toConsoleBufferNS(std::string & str, float x, float y, int colour);

	void toConsoleBufferNS(std::string & str, float x, float y);

	void toConsoleBufferNS(std::vector<std::string>& str, float & poX, float & poY, float x, float y, int colour);

	void toConsoleBufferNS(std::vector<std::string>& str, float & poX, float & poY, float x, float y);

	void toConsoleBufferNS(std::vector<std::string>& str, float x, float y, int colour);

	void toConsoleBufferNS(std::vector<std::string>& str, float x, float y);

	void toConsoleBufferNS(const char * str, float & poX, float & poY, float x, float y, int colour);

	void toConsoleBufferNS(const char * str, float & poX, float & poY, float x, float y);

	void toConsoleBufferNS(const char * str, float x, float y, int colour);

	void toConsoleBufferNS(const char * str, float x, float y);

	void toConsoleBuffer(std::string& str, float& poX, float& poY, float x, float y, int colour);

	void toConsoleBuffer(std::string& str, float& poX, float& poY, float x, float y);

	void toConsoleBuffer(std::string& str, float x, float y, int colour);

	void toConsoleBuffer(std::string& str, float x, float y);

	void toConsoleBuffer(std::string & str, float & poX, float & poY, float x, float y, std::vector<int> &colour);

	void toConsoleBuffer(std::vector<std::string>& str, float x, float y, std::vector<std::vector<int>> &colour);

	void toConsoleBuffer(std::vector<std::string>& str, float& poX, float& poY, float x, float y, int colour);

	void toConsoleBuffer(std::vector<std::string>& str, float& poX, float& poY, float x, float y);

	void toConsoleBuffer(std::vector<std::string>& str, float x, float y, int colour);

	void toConsoleBuffer(std::vector<std::string>& str, float x, float y);

	void toConsoleBuffer(const char * str, float& poX, float& poY, float x, float y, int colour);

	void toConsoleBuffer(const char * str, float& poX, float& poY, float x, float y);

	void toConsoleBuffer(const char * str, float x, float y, int colour);

	void toConsoleBuffer(const char * str, float x, float y);

	int leftButtonPress();

	bool leftButtonRelease();

	bool rightButtonPress();

	bool rightButtonRelease();

	COORD mouseLocation();

	void drawConsole(bool clear = true);

	void clearConsole();

	/*Variables*/

	CHAR_INFO *ci;
	CHAR_INFO **ci2;
	COORD cursorPosition;
	HANDLE con[2];
	INPUT_RECORD inputRecord[128];
	COORD sz = GetLargestConsoleWindowSize(con[1]);
	SMALL_RECT SIZE;
	DWORD cCharsWritten;
	float conWidth, conHeight;
	bool counter = 0, resizable;

};
