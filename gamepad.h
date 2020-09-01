#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "SDL/SDL.h"

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

class GamePads
{
public:
	SDL_GameController* controllers[4];
	int numberConectedJoysticks;

	PlayerInput inputs[4];

	GamePads()
	{
		SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS);
		updateControllers();
	}

	void openControllers(void);
	void resetControllers(void);
	void updateControllers(void);

};



#endif // !GAMEPAD_H