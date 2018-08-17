/*************************************************
**Name     : Emory Wynn							**
**Student# : 100655604							**
**************************************************/

#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <fcntl.h>
#include <io.h>

#define newstr new char
typedef unsigned short ushort;
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
	FG_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
	FG_RED = FOREGROUND_RED,
	FG_PURPLE = FOREGROUND_BLUE | FOREGROUND_RED,
	FG_YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
	FG_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
	FG_INTENSIFY = FOREGROUND_INTENSITY,
	//FG_UNDERLINE = UNDERLINE,

	BG_BLUE = BACKGROUND_BLUE,
	BG_GREEN = BACKGROUND_GREEN,
	BG_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
	BG_RED = BACKGROUND_RED,
	BG_PURPLE = BACKGROUND_BLUE | BACKGROUND_RED,
	BG_YELLOW = BACKGROUND_GREEN | BACKGROUND_RED,
	BG_WHITE = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
	BG_INTENSIFY = BACKGROUND_INTENSITY

};

enum MouseButtons
{
	LEFT_CLICK = FROM_LEFT_1ST_BUTTON_PRESSED,
	RIGHT_CLICK = RIGHTMOST_BUTTON_PRESSED,
	MIDDLE_CLICK = FROM_LEFT_2ND_BUTTON_PRESSED,
	UNKNOWN_CLICK_1 = FROM_LEFT_3RD_BUTTON_PRESSED,
	UNKNOWN_CLICK_2 = UNKNOWN_CLICK_1 * 2
};

/*Structs*/
struct Sprite
{
	/*
	Takes a .txt file and outputs into the Sprite.
	The sprite will be overridden once this is called
	*/
	void create(const char* file, char* tag = nullptr)
	{
		setTag(tag);
		FILE *f;
		fopen_s(&f, file, "r, ccs=UNICODE");

		//std::vector<std::wstring>* tmp = _sprite;
		_sprite->clear();

		std::vector<std::wstring>sprite;

		_height = 
		_width = 0;

		wchar_t * str = new wchar_t[255];
		std::wstring str2 = L"";
		//while(str2 = fgetws(str2, 255, f),
		//	  str = (str2 == nullptr ? L"" : (str2[wcslen(str2) - 1] = '\0', str2)),
		//	  str2 != nullptr);

		while(str = fgetws(str, 255, f), sprite.push_back(str2=( str == nullptr ? L"" : (str[wcslen(str) - 1] = '\0',str))), str != nullptr)
			_width = _width < (sprite[_height]).size() ? (sprite[_height]).size() : _width,
		_height++;

		*_sprite = sprite;

		fclose(f);
	}

	void create(std::vector<std::wstring>* sprite, char* tag = nullptr)
	{
		setTag(tag);

		_sprite->clear();
		_height = sprite->size();
		*_sprite = *sprite;
		for(int a = 0; a < _height; a++)
			_width = _width < _sprite[0][a].size() ? _sprite[0][a].size() : _width;
	}
	/*

	************
	  ********     *******
		****
		 **

	*/
	bool boxCollision(Sprite s2, COORD p1, COORD p2)
	{
		p1 = {p1.X + _width / 2,p1.Y + _height / 2};
		p2 = {p2.X + s2._width / 2,p2.Y + s2._height / 2};

		if(abs(p1.X - p2.X) <= _width / 2 + s2._width / 2)
			if(abs(p1.Y - p2.Y) <= _height / 2 + s2._height / 2)
				return true;
		return false;
	}

	void setTag(char* tag)
	{
		_tag = tag;
	}

	void setColour(short colour)
	{
		_colour = colour;
	}

	const char* getTag()
	{
		return _tag;
	}

	std::wstring* getSprite()
	{
		return _sprite->data();
	}

	unsigned short getWidth()
	{
		return _width;
	}

	unsigned short getHeight()
	{
		return _height;
	}

private:
	std::vector<std::wstring>* _sprite = new std::vector<std::wstring>;
	char *_tag = nullptr;
	ushort _width = 0, _height = 0;
	short _colour = 0;
};

struct SpriteSheet
{
	/*
	Creates SpriteSheet from a .txt file. Creates a new
	Sprite when it finds a line with a lenght of zero
	*/
	void create(const char* file)
	{
		FILE *f;

		//fopen_s(&f, file, "r");
		int size = 0, count = 0;
		wchar_t* str2 = new wchar_t[255];

		//gets the largest sprite height
		//while(count++, fgets(str2, 255, f) != nullptr)
		//	if(strlen(str2) == 1)
		//		size = size < --count ? count : size, count = 0;
		//if(count != 0)
		//	size = size < --count ? count : size, count = 0;
		//fclose(f);

		std::vector<std::wstring> sprite;
		unsigned short width = 0, height = 0;
		bool seg = 0;
		std::wstring str;

		fopen_s(&f, file, "r, ccs=UNICODE");

		while(str2 = fgetws(str2, 255, f),
			  str = (str2 == nullptr ? L"" : (str2[wcslen(str2) - 1] = '\0', str2)),
			  str2 != nullptr)
		{
			if(str.size() == 0)
			{
				if(!seg)
				{
					add(&sprite);

					width = 0, height = 0;
					sprite.clear();
				}
				seg = true;
				continue;
			}

			width = width < str.size() ? str.size() : width;
			sprite.push_back(str);
			height++;
			seg = false;
		}
		fclose(f);

		if(height > 0)
			add(&sprite);
		delete str2;
	}

	void add(Sprite* sprite)
	{
		//Sprite **tmp = new Sprite*[++_numSprites],
		//	**tmp2 = m_sheet;
		//
		////copy data
		//for(int a = 0; a < _numSprites - 1; a++)
		//	tmp[a] = new Sprite,
		//	*(tmp[a]) = *(m_sheet[0][a]),
		//	delete m_sheet[0][a];
		//
		////add sprite to sprite sheet
		//tmp[_numSprites - 1] = new Sprite;
		//tmp[_numSprites - 1] = sprite;
		////m_sheet->push_back( new Sprite);
		////m_sheet[0][0][_numSprites - 1] = sprite;
		//m_sheet = tmp;

		_sheet->push_back(sprite);
	}

	void add(std::vector<std::wstring>* sprite)
	{
		//Sprite
		//	**tmp = new Sprite*[++_numSprites];
		//	//**tmp2 = m_sheet;
		//
		////cpy data
		//for(int a = 0; a < _numSprites - 1; a++)
		//	tmp[a] = new Sprite,
		//	*tmp[a] = *m_sheet[0][a],
		//	delete m_sheet[0][a];
		////delete[] tmp2;
		//
		////add sprite to sprite sheet
		//tmp[_numSprites - 1] = new Sprite;
		//tmp[_numSprites - 1]->create(sprite);
		////m_sheet->push_back(new Sprite);
		////m_sheet[0][0][_numSprites++]->create(sprite);
		//m_sheet = tmp;
		Sprite* tmp = new Sprite;
		tmp->create(sprite);
		_sheet->push_back(tmp);

	}

	void remove(int index)
	{
		//delete m_sheet[0][index];
		//_numSprites--;
		_sheet->erase(_sheet->begin() + index);

	}

	void remove(const char* tag)
	{
		for(int a = 0; a < size(); a++)
			if(_sheet[0][a]->getTag() == tag)
			{

				//delete m_sheet[0][a];
				//break;
				_sheet->erase(_sheet->begin() + a);
			}

		//_numSprites--;
	}

	void clear()
	{
		//for(int a = 0; a < _numSprites; a++)
		//	delete m_sheet[0][a];
		//Sprite** tmp = m_sheet;
		//delete[] tmp;

		_sheet->clear();
	}

	int size()
	{
		return _sheet->size();
	}

	Sprite& at(unsigned int index)
	{
		return operator[](index);
	}

	Sprite& at(const char* tag)
	{
		for(int a = 0; a < size(); a++)
			if(_sheet[0][a]->getTag() == tag)
				return *_sheet[0][a];
		return *_sheet[0][size()];
	}

	Sprite& operator[](unsigned int index)
	{
		return *_sheet[0][index];
	}
private:
	std::vector<Sprite*>* _sheet = new std::vector<Sprite*>;
	//unsigned short _numSprites = 0;
};

struct Animation
{
	void create(SpriteSheet& frames)
	{
		*m_sheet = frames;
	}

	Sprite animate(bool repeat = true)
	{

	}

	void reset()
	{}

	void setFPS(ushort fps)
	{}

	void setCurrentFrame(ushort frame)
	{
		m_currentFrame = frame;
	}
private:
	SpriteSheet * m_sheet = new SpriteSheet;
	ushort m_fps, m_currentFrame;
};

struct MouseInput
{
	COORD position{0,0};
	short vertWheel, horiWheel;
	std::map <short, bool> buttons;
	bool doubleClick;

};

/*Classes*/
class EmConsole
{
public:
	/***Constructers***/
	EmConsole(std::string& title);

	EmConsole(const char* title);

	EmConsole();

	void setFullScreen(bool);

	/***Functions***/

	//sends a text file to a vector of vector wstring 
	void textFileToVector(std::string file, std::vector<std::vector<std::wstring>> &str);

	//sends a text file to a vector of vector wstring
	std::vector<std::vector<std::wstring>> textFileToVector(std::string file);

	//sets the title
	void setTitle(std::string title);

	//sets the console size
	void setConsoleSize(float x, float y);

	//returns
	COORD getConsoleSize();

	int getWidth();

	int getHeight();

	void printf(const char *...);

	//sets weather or not the window can be resized
	void setResizable(bool resz);

	//sets position of the console from the top left corner
	void setConsolePosition(float x, float y);

	void placeConsoleCenter();

	void consoleCursorPosition(float x, float y);

	/* Start: special stuff for tetris (don't even try to understand)*/
	std::vector<int> readConsoleLineAtributes(float x, float y, float width);

	std::vector<std::vector<int>> readConsoleAreaAtributes(float x, float y, float width, float height);

	std::vector<std::wstring> readConsoleArea(float x, float y, float width, float height);

	std::wstring readConsoleLine(float x, float y, float width);

	char readConsoleCharacter(float x, float y);

	char readActiveConsoleCharacter(float x, float y);
	/* End: special stuff for tetris (don't even try to understand)*/

	MouseInput mouseState()
	{
		//static const char* button = new const char[128];
		DWORD numEvents;
		INPUT_RECORD irBuff[128];
		static std::map < DWORD, bool > buttons;
		static MouseInput input;
		//SetConsoleMode(hStdin, newInputMode);



		for(int count = 0; count < 150; count++)
		{
			GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
			if(numEvents)
				ReadConsoleInputA(
				GetStdHandle(STD_INPUT_HANDLE),      // input buffer handle 
				irBuff,      // buffer to read into 
				128,         // size of read buffer 
				&numEvents);

			for(int a = 0; a < numEvents; a++)
			{
				switch(irBuff[a].EventType)
				{
				case MOUSE_EVENT: // mouse input 
					MOUSE_EVENT_RECORD mer = irBuff[a].Event.MouseEvent;
					switch(mer.dwEventFlags)
					{
					case DOUBLE_CLICK:
						input.doubleClick = true;
						break;
					case 0:
						input.doubleClick = false;
						for(int b = 1; b < 0x10; b *= 2)
							input.buttons[b] = (b == mer.dwButtonState);

						break;

					case MOUSE_MOVED:
						input.position = {mer.dwMousePosition.X, mer.dwMousePosition.Y};
						break;
					case MOUSE_WHEELED://vertical
						input.vertWheel += mer.dwButtonState;
						break;
					case MOUSE_HWHEELED://horizontal
						input.horiWheel += mer.dwButtonState;
						break;
					}
					break;
				}
			}
		}
		//if(*button != -51)
		//	return button;
		return input;
	}



	/*Console buffer overloads*/

	/*
	Note: NS means No Spaces, wrighting only characters excluding spaces.
	Extrymely inefficient
	*/

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, float x, float y, Colour colour);
	* str    - wstring to be drawn to buffer
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
	void toConsoleBufferNS(std::wstring & str, float & poX, float & poY, float x, float y, int colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, float x, float y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(std::wstring & str, float & poX, float & poY, float x, float y);

	/*
	toConsoleBuffer(wstring &str, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBufferNS(std::wstring & str, float x, float y, int colour);

	/*
	toConsoleBuffer(wstring &str, float x, float y);
	* str    - wstring to be drawn
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(std::wstring & str, float x, float y);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, float x, float y, int colour);
	* str    - vector of wstring to be drawn to buffer
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
	void toConsoleBufferNS(Sprite& str, float & poX, float & poY, float x, float y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, float x, float y);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(Sprite& str, float & poX, float & poY, float x, float y);

	/*
	toConsoleBuffer(vector<wstring>& str, float x, float y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBufferNS(Sprite& str, float x, float y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float x, float y);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(Sprite& str, float x, float y);

	/*
	toConsoleBuffer(const char* str, float poX, float poY, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
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
	void toConsoleBufferNS(const wchar_t * str, float & poX, float & poY, float x, float y, int colour);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, float x, float y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(const wchar_t * str, float & poX, float & poY, float x, float y);

	/*
	toConsoleBuffer(const wchar_t *str, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBufferNS(const wchar_t * str, float x, float y, int colour);

	/*
	toConsoleBuffer(const wchar_t *str, float x, float y);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBufferNS(const wchar_t * str, float x, float y);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
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
	void toConsoleBuffer(std::wstring & str, float & poX, float & poY, float x, float y, std::vector<int> &colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
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
	void toConsoleBuffer(std::wstring& str, float& poX, float& poY, float x, float y, int colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, float x, float y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(std::wstring& str, float& poX, float& poY, float x, float y);

	/*
	toConsoleBuffer(wstring &str, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBuffer(std::wstring& str, float x, float y, int colour);

	/*
	toConsoleBuffer(wstring &str, float x, float y);
	* str    - wstring to be drawn
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(std::wstring& str, float x, float y);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, float x, float y, std::vector<std::vector<int>> &colour);
	* str    - vector of wstring to be drawn to buffer
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
	void toConsoleBuffer(Sprite& str, float x, float y, std::vector<std::vector<int>> &colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, float x, float y, int colour);
	* str    - vector of wstring to be drawn to buffer
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
	void toConsoleBuffer(Sprite& str, float& poX, float& poY, float x, float y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, float x, float y);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(Sprite& str, float& poX, float& poY, float x, float y);

	/*
	toConsoleBuffer(vector<wstring>& str, float x, float y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBuffer(Sprite& str, float x, float y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float x, float y);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(Sprite& str, float x, float y);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
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
	void toConsoleBuffer(const wchar_t * str, float& poX, float& poY, float x, float y, int colour);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, float x, float y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(const wchar_t * str, float& poX, float& poY, float x, float y);

	/*
	toConsoleBuffer(const wchar_t *str, float x, float y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	void toConsoleBuffer(const wchar_t * str, float x, float y, int colour);

	/*
	toConsoleBuffer(const wchar_t *str, float x, float y);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	*/
	void toConsoleBuffer(const wchar_t * str, float x, float y);

	/*
	drawConsole(bool clear = true);
	* clear = clear each frame

	Summary:
	Draws the console (choose to clear console buffer after)
	*Note:
	Default is true
	*/
	void drawConsole(bool clear = true);

	//clears the console
	void clearConsole();

private:
	/**Variables**/

	CHAR_INFO * _ci;
	CHAR_INFO **_ci2;
	DWORD oldInputMode, newInputMode;

	COORD _cursorPosition;
	HANDLE _con[2], _input;
	INPUT_RECORD _inputRecord[128];
	COORD _sz = {0,0};
	float _conWidth, _conHeight;
	bool _buff = 0, _resizable, _full = 0;

};
