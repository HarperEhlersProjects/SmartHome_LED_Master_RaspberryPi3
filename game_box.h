#ifndef GAME_BOX_H
#define GAME_BOX_H

#include "graphics.h"

using namespace gameobjects;

#define GAMEBOX_FLOATS_NUMBER 20
#define GAMEBOX_OBJECT_ID_NUMBER 20
#define GAMEBOX_BOOLS_NUMBER 20
#define GAMEBOX_INTEGERS_NUMBER 20

enum GameBoxState {
	GBStateInit,
	GBStateMainmenu,
	GBStatePong,
	GBStateBricks
	
};

typedef struct {
	bool SELECT;
	bool START;
	
	bool A;
	bool B;
	bool X;
	bool Y;

	bool UP;
	bool DOWN;
	bool LEFT;
	bool RIGHT;

	bool L;
	bool R;
}PlayerInput;

class GameBox
{
public:
	GameBoxState state, nextState;
	bool initialAttempt;
	ObjectCollection objects;
	PlayerInput playerInputs[2];

	tsObjectID objectIDs[GAMEBOX_OBJECT_ID_NUMBER];
	float floats[GAMEBOX_FLOATS_NUMBER];
	bool bools[GAMEBOX_BOOLS_NUMBER];
	int integers[GAMEBOX_INTEGERS_NUMBER];

	GameBox()
	{
		state = GBStateInit;
		nextState = GBStatePong;
		objects = ObjectCollection();
		resetGameBox();

	}

	void InitGamepads(void);

	void calculateStep();
	void returnToMainMenu();


private:
	void resetGameBox(void);

	void mainMenu(void);

	//Declare new Gamemethods here
	void pong(void);
	void bricks(void);

};




#endif /* GAME_BOX_H */