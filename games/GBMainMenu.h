#ifndef GBMAINMENU_H
#define GBMAINMENU_H

#pragma once

#include "game_engine/game_engine.h"

class GBMainMenu : public Game
{
public:
	Text title, pongText, snakeText;
	Rectangle underline, rightBorder;

	bool buttonUP, buttonDOWN;
	int currentSelection;

	GBMainMenu();
	void calculateStep(void);
};

#endif // !GBMAINMENU_H