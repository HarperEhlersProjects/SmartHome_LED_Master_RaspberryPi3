#ifndef GAME_BOX_H
#define GAME_BOX_H

#include "graphics.h"

using namespace gameobjects;

#define GAMEBOX_ACTORS_NUMBER 10


enum GameBoxState {

	mainmenu
	
};

class GameBox
{
public:
	GameBoxState state;
	ObjectCollection objects;
	float gameboxActors[GAMEBOX_ACTORS_NUMBER];

	GameBox()
	{
		objects = ObjectCollection();
		gameboxActors[0] = 0;

	}

	void CalculateStep();

};




#endif /* GAME_BOX_H */