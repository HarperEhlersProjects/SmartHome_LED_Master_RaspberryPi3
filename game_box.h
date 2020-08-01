#ifndef GAME_BOX_H
#define GAME_BOX_H

#include "graphics.h"

using namespace gameobjects;

#define GAMEBOX_FLOATS_NUMBER 20
#define GAMEBOX_OBJECT_ID_NUMBER 20
#define GAMEBOX_BOOLS_NUMBER 20


enum GameBoxState {
	GBStateInit,
	GBStateMainmenu,
	GBStatePong
	
};

class GameBox
{
public:
	GameBoxState state, nextState;
	bool initialAttempt;
	ObjectCollection objects;



	tsObjectID objectIDs[GAMEBOX_OBJECT_ID_NUMBER];
	float floats[GAMEBOX_FLOATS_NUMBER];
	bool bools[GAMEBOX_BOOLS_NUMBER];

	GameBox()
	{
		state = GBStateInit;
		nextState = GBStateMainmenu;
		objects = ObjectCollection();
		resetGameBox();

	}

	void calculateStep();
	void returnToMainMenu();

private:
	void resetGameBox(void);

	void mainMenu(void);

	//Declare new Gamemethods here
	void pong(void);

};




#endif /* GAME_BOX_H */