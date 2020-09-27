#ifndef GAME_BOX_H
#define GAME_BOX_H

#pragma once

#include "game_engine/game_engine.h"


class GameBox
{
public:
	Game* game;
	GameType state;
	GamePads gamepads;

	GameBox() :
		game(nullptr), state(GTypeMainmenu), gamepads(GamePads()) {}

	void calculateStep();
	void returnToMainMenu();

	void draw(DPU* display);
};

#endif /* GAME_BOX_H */