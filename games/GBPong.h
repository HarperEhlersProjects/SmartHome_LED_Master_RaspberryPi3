#ifndef GBPONG_H
#define GBPONG_H

#pragma once

#include "game_engine/game_engine.h"

class GBPong : public Game
{
public:

	Rectangle ball, wallS, wallN, wallE, wallW , player1, player2;

	GBPong(GamePads* gamepads);

	void calculateStep(void);
};

#endif // !GBPONG_H