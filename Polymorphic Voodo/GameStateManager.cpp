#include "GameStateManager.h"


using namespace std;
GameStateManager::GameStateManager()
{}


GameStateManager::~GameStateManager()
{
	delete handelPtr;
}

GameStateManager* GameStateManager::handelPtr = nullptr;

GameStateManager* GameStateManager::getGameStateManager()
{
	if(handelPtr == nullptr)
		handelPtr = new GameStateManager();
	return handelPtr;
}

void GameStateManager::addGameState(std::string str, GameState* state)
{
	states[str] = state;
	state->parent = this;
}

void GameStateManager::removeGameStates(std::string str)
{
	states[str] = nullptr;
}

GameState* GameStateManager::getGameState(std::string str)
{
	if(states[str] != NULL)
		return states[str];
	printf("ERROR, NO GAMESTATE WITH NAME %s", str.c_str());
}

void GameStateManager::update()
{
	for(auto &ud : states)
	{
		if(ud.second != nullptr && ud.second->paused != true)
			ud.second->update();
	}
}

void GameState::setPaused(bool passud)
{
	paused = passud;
}