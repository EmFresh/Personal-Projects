#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

class GameState;

class GameStateManager
{
public:
	~GameStateManager();
	static GameStateManager* getGameStateManager();
	void addGameState(std::string,GameState*);
	void removeGameStates(std::string);
	GameState* getGameState(std::string);
	void update();

private:
	GameStateManager();
	static GameStateManager* handelPtr;
	std::unordered_map<std::string, GameState*> states;
};

class GameState
{
public:
	virtual void update() = 0;
	void setPaused(bool);
	bool paused = false;
	GameStateManager* parent;
};