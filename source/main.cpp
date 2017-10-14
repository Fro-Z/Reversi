#include <ncurses.h>
#include "GameEngine.h"

#include <stdlib.h>     //rand for AI
#include <time.h>

#include <iostream>
#include <signal.h>

void sigpipeHandler(int sig)
{
	GameEngine& engine = GameEngine::GetInstance();
	engine.ShowMainMenu();
	engine.DisplayPopup("Disconnected");
}

int main()
{
	signal(SIGPIPE, sigpipeHandler);

	srand(static_cast<unsigned int>(time(NULL)));

	GameEngine& engine = GameEngine::GetInstance();

	engine.Initialize();

	//main loop
	while(engine.GetGameState()!=GameEngine::STATE_QUIT)
	{
		engine.Think();

	}
}
