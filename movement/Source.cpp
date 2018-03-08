#include <iostream>
#include <chrono>
#include <thread>
#define pauseThread(x) this_thread::sleep_for(chrono::milliseconds(x))
using namespace std;

void moveEnemy(int playerPos, int &enemyPos)
{
	if(playerPos > enemyPos)
	{
		enemyPos++;
	} else
	{
		enemyPos--;
	}
}

char thing[] = {'x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','o'};
void main()
{
	int enemyPos=0;
	do
	{
		moveEnemy(12, enemyPos);
		swap(thing[enemyPos - 1], thing[enemyPos]);
		for(int a = 0; a < 13; a++)
			printf("%c, ",thing[a]);		
		pauseThread(1000);
		system("cls");
	} while(true);
}