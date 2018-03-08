#include "GameStates.h"
using namespace std;
int main()
{
	bool done = false;

	MainMenu* mainMenu = new MainMenu;
	ManMenu* manMenu = new ManMenu;
	GameLevel* gameLevel = new GameLevel;

	GameStateManager::getGameStateManager()->addGameState("MainMenu", mainMenu);
	GameStateManager::getGameStateManager()->addGameState("ManMenu",manMenu);
    GameStateManager::getGameStateManager()->addGameState("GameLevel",gameLevel);

	mainMenu->setPaused(false);
	manMenu->setPaused(true);
	gameLevel->setPaused(true);

	while(!done)
	{
		GameStateManager::getGameStateManager()->update();
	}


	return 0;
}