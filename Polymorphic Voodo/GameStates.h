#pragma once
#include"GameStateManager.h"
class MainMenu :public GameState
{
public:
	MainMenu();
	~MainMenu();

	void enter();
	void exit();
	void update();//needs to be the same as the GameStatesManager
private:
	bool hasBeeninitialized;
};

class ManMenu :public GameState
{
public:
	ManMenu();
	~ManMenu();

	void enter();
	void exit();
	void update();//needs to be the same as the GameStatesManager
private:
	bool hasBeeninitialized;
};

class GameLevel :public GameState
{
public:
	GameLevel();
	~GameLevel();

	void enter();
	void exit();
	void update();//needs to be the same as the GameStatesManager
private:
	bool hasBeeninitialized;
};