#ifndef GBSNAKE_H
#define GBSNAKE_H

#pragma once

#include "game_engine/game_engine.h"
#include <vector>

enum Direction{
	DOWN,
	UP,
	LEFT,
	RIGHT
};

class GBSnake : public Game
{
public:
	tsPosition head, temp;
	Rectangle fruit;
	std::vector<Rectangle> snake;
	
	Rectangle grid[16][16];

	bool buttonLeft, buttonRight, fruitOnSnake;

	Direction dir;
	int timer;

	GBSnake(GamePads* gamepads);
	void calculateStep(void);
};

#endif // !GBSNAKE_H