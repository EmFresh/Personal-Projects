#include "GameStates.h"


using namespace std;
//MAIN MENU

MainMenu::MainMenu()
{
	hasBeeninitialized = false;
}

MainMenu::~MainMenu()
{}

void MainMenu::enter()
{
	if(paused)
	{
		paused = false;
		MainMenu::setPaused(false);
	}
	//setup, initialize scene
}

void MainMenu::exit()
{
	hasBeeninitialized = false;
	if(!paused)
	{
		paused = true;
		MainMenu::setPaused(true);   //Pause the CURRENT STATE
		MainMenu(hasBeeninitialized);//Prep the NEXT STATE for initialization
	}
}

void MainMenu::update()
{
	if(!hasBeeninitialized)
	{
		enter();
		hasBeeninitialized = true;
	}
	printf("Main Menu\n");
	MainMenu::exit();
	parent->getGameState("ManMenu")->setPaused(false);
}

//MAN MENU
ManMenu::ManMenu()
{
	hasBeeninitialized = false;
}

ManMenu::~ManMenu()
{}

void ManMenu::enter()
{
	if(paused)
	{
		paused = false;
		ManMenu::setPaused(false);
	}
	//setup, initialize scene
}

void ManMenu::exit()
{
	hasBeeninitialized = false;
	if(!paused)
	{
		paused = true;
		ManMenu::setPaused(true);   //Pause the CURRENT STATE
		ManMenu(hasBeeninitialized);//Prep the NEXT STATE for initialization
	}
}

void ManMenu::update()
{
	if(!hasBeeninitialized)
	{
		enter();
		hasBeeninitialized = true;
	}
	printf("Man Menu\n");
	ManMenu::exit();
	parent->getGameState("GameLevel")->setPaused(false);
}
//GAME MENU
GameLevel::GameLevel()
{
	hasBeeninitialized = false;
}

GameLevel::~GameLevel()
{}

void GameLevel::enter()
{
	if(paused)
	{
		paused = false;
		GameLevel::setPaused(false);
	}
	//setup, initialize scene
}

void GameLevel::exit()
{
	hasBeeninitialized = false;
	if(!paused)
	{
		paused = true;
		GameLevel::setPaused(true);   //Pause the CURRENT STATE
		GameLevel(hasBeeninitialized);//Prep the NEXT STATE for initialization
	}
}

void GameLevel::update()
{
	if(!hasBeeninitialized)
	{
		enter();
		hasBeeninitialized = true;
	}

	printf("Game Level\n");
	GameLevel::exit();
	parent->getGameState("MainMenu")->setPaused(false);
}