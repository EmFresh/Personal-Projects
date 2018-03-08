#include "EmConsole.h"
#include "keyinput.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <time.h>

using namespace std;

#define pauseThread(x) this_thread::sleep_for(chrono::milliseconds(x))
keyinput key;
Console con;
vector<vector<vector<string>>> shapes, shapeHolder;
vector<vector<string>>board;
vector<vector<int>>tempBoardAtra(50, vector<int>(120, 11));
vector<int> x(1, 5), y(1, 2), rotation(1, 0), colour;
fstream tempBoard("tempBoard.txt", fstream::out);
int movementCount , colourNum;

bool colisionFull(int x, int y, vector<string> &shape)
{
	for(int a = shape.size() - 1; a >= 0; a--)
	{
		for(int b = 0; b < shape[a].size(); b++)
		{
			if(shape[a][b] != ' ')
				if(con.readConsoleCharacter(x + b, y + a) != ' ')
					return 1;
		}
	}
	return 0;
}

bool colisionBottom(int x, int y, vector<string> &shape)
{
	int  b = 0;
	for(int a = shape.size() - 1; a >= 0; a--)
	{
		while(b < shape[a].size())
		{
			if(shape[a][b] != ' ')
			{
				if(con.readConsoleCharacter(x + b, y + a) != ' ')
					return 1;
				else
				{
					b++;
					a = shape.size();
					break;
				}
			} else
			{
				if(a == 0 && b < shape[a].size())
				{
					b++;
					a = shape.size();

				}
				break;
			}
		}
	}

	return 0;
}

bool colisionLeft(int x, int y, vector<string> &shape)
{

	for(int a = shape.size() - 1; a >= 0; a--)
	{
		for(int b = 0; b < shape[a].size(); b++)
		{
			if(shape[a][b] != ' ')
				if(con.readConsoleCharacter(x + b, y + a) != ' ')
					return 1;
				else
					break;

		}
	}
	return 0;
}

bool colisionRight(int x, int y, vector<string> &shape)
{
	for(int a = shape.size() - 1; a >= 0; a--)
	{
		for(int b = shape[a].size() - 1; b >= 0; b--)
		{
			if(shape[a][b] != ' ')
				if(con.readConsoleCharacter(x + b, y + a) != ' ')
					return 1;
				else
					break;
		}
	}
	return 0;
}

void deleteLines()
{
	//"************************************************************************************************************";
	//string line;
	for(int a = 46; a > 2; a--)
	{
		if((con.readConsoleLine(5, a, 108)) == "************************************************************************************************************")
		{			
			con.toConsoleBuffer(con.readConsoleArea(5, 2, 108, a - 2), 5, 3, con.readConsoleAreaAtributes(5, 2, 108, a - 2));
			a++;
		}
	}
	//OutputDebugStringA("end\n\n");
}

void shapeMovement()
{
	bool check = 0;

	if(key.press(VK_LEFT) && !colisionLeft(x[0] - 6, y[0], shapes[0][rotation[0]]))
		x[0] -= 6;
	else if(key.press(VK_RIGHT) && !colisionRight(x[0] + 3, y[0], shapes[0][rotation[0]]))
		x[0] += 6;
	else if(key.press(VK_DOWN))
	{
		y[0] += 2;
		check = 1;
	}

	

	if(key.stroke('A'))
	{
		if(rotation[0] > 0)
		{
			if(!colisionFull(x[0], y[0], shapes[0][rotation[0] - 1]))
				rotation[0]--;
		} else
			if(!colisionFull(x[0], y[0], shapes[0][shapes[0].size() / 2 - 1]))
				rotation[0] = shapes[0].size() / 2 - 1;
	} else if(key.stroke('D'))
		if(rotation[0] < shapes[0].size() / 2 - 1)
		{
			if(!colisionFull(x[0], y[0], shapes[0][rotation[0] + 1]))
				rotation[0]++;
		} else
			if(!colisionFull(x[0], y[0], shapes[0][0]))
				rotation[0] = 0;



		if(!key.press(VK_DOWN) && movementCount++ == 5)
		{
			y[0]++;
			movementCount = 0;
			check = 1;
		}

		if(check && colisionBottom(x[0], y[0], shapes[0][rotation[0]]))
		{

			while(colisionBottom(x[0], y[0], shapes[0][rotation[0]]))
				y[0]--;
			con.toConsoleBuffer(con.textFileToVector("tempBoard.txt")[0], 0, 0, tempBoardAtra);
			con.toConsoleBufferNS(shapes[0][rotation[0]], x[0], y[0], colour[colourNum]);
			deleteLines();
			tempBoard.open("tempBoard.txt", fstream::out);
			for(int a = 0; a < 50; a++)
				tempBoard << con.readConsoleLine(0, a, 120) << endl;
			tempBoard.close();
			tempBoardAtra = con.readConsoleAreaAtributes(0, 0, 120, 50);



			//x.insert(x.begin(), x[0]);
			y[0] = 2;
			shapes[0] = shapeHolder[colourNum = (rand() % shapeHolder.size())];
			rotation[0] = 0;

			if(colisionFull(x[0], y[0], shapes[0][rotation[0]]))
			{
				if(colisionLeft(x[0], y[0], shapes[0][rotation[0]]))

					while(colisionLeft(x[0], y[0], shapes[0][rotation[0]]))
						x[0]++;
				else
					while(colisionFull(x[0], y[0], shapes[0][rotation[0]]))
						x[0]--;
			}
		}
}

void restart()
{
	shapes.clear();
	x.clear();
	y.clear();
	rotation.clear();
	tempBoardAtra.clear();

	shapes.push_back(shapeHolder[colourNum=(rand() % shapeHolder.size())]);
	x.push_back(5);
	y.push_back(2);
	rotation.push_back(0);
	tempBoardAtra.assign(50, vector<int>(120, 11));
	
	tempBoard.open("tempBoard.txt", fstream::out);
	tempBoard << "";
	//tempBoard.flush();
	//tempBoard.clear();
	tempBoard.close();
}

void main()
{
	srand(time(NULL));
	tempBoard.close();
	board = con.textFileToVector("Tetris Board.txt");

	shapeHolder.push_back(con.textFileToVector("shape1.txt"));
	shapeHolder.push_back(con.textFileToVector("shape2.txt"));
	shapeHolder.push_back(con.textFileToVector("shape3.txt"));
	shapeHolder.push_back(con.textFileToVector("shape4.txt"));

	colour.push_back(con.FG_RED | con.FG_INTENSITY);
	colour.push_back(con.FG_PURPLE | con.FG_INTENSITY);
	colour.push_back(con.FG_YELLOW | con.FG_INTENSITY);
	colour.push_back(con.FG_GREEN | con.FG_INTENSITY);
	shapes.push_back(shapeHolder[colourNum=(rand() % shapeHolder.size())]);
	con.setConsoleSize(board[0][0].size(), board[0].size());
	//HMODULE sounds = Gethmoddual;
	//mciSendString("tetris remix.wav", NULL, 0, NULL); 

	while(!key.stroke('Q'))
	{

		con.toConsoleBuffer(board[0], 0, 0, con.FG_CYAN | con.FG_INTENSITY);
		con.toConsoleBuffer(con.textFileToVector("tempBoard.txt")[0], 0, 0, tempBoardAtra);
		//for(int a = shapes.size() - 1; a > 0; a--)
		//	con.toConsoleBufferNS(shapes[a][rotation[a]], x[a], y[a], colour[]);

		shapeMovement();

		con.toConsoleBufferNS(shapes[0][rotation[0]], x[0], y[0],colour[colourNum]);

		con.drawConsole();
		pauseThread(10);
		if(key.stroke('R'))
			restart();
	}

	//int things[2][2] {{5,5},{6,5}};
	//int things2[5][2][2] {things,things,things,things,things};
}