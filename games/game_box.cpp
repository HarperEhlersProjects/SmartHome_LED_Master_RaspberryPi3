#include "game_box.h"
#include "GBMainMenu.h"
#include "GBPong.h"

void GameBox::calculateStep()
{
	//Multiplex the current state of game box.

	if (game == nullptr)
	{
		switch (state)
		{
		case GTypePong:
			game = new GBPong();
			break;
		default:
			game = new GBMainMenu();
		}
	}
	else
	{
		if (game->nextState != state)
		{
			returnToMainMenu();
		}
		else
		{
			//Update controller inputs
			game->gamepads.updateControllers();
			//Calculate one time step of the game
			game->calculateStep();
		}
	}
}

//Change the next state to main menu.
void GameBox::returnToMainMenu()
{
	state = game->nextState;
	delete game;
	game = nullptr;
}

void GameBox::draw(DPU* display)
{
	if (game != nullptr)
	{
		game->drawObjects(display);
	}
}