#include "game_box.h"
#include "SDL/SDL.h"


//#define MAX_CONTROLLERS 4
//
//class JoypadController {
//public:
//	JoypadController() : is_connected(false), gamepad(0), instance_id(-1), haptic(0) {}
//
//	int processEvent(const SDL_Event& event);
//private:
//	SDL_GameController* gamepad;
//	SDL_Haptic* haptic;
//	SDL_JoystickID instance_id;
//	bool is_connected;
//
//	static JoypadController ms_controllers[MAX_CONTROLLERS];
//	static int GetControllerIndex(SDL_JoystickID instance);
//
//	void Open(int device);
//	void Close();
//
//};
//
//// Opens the joystick controller
//void JoypadController::Open(int device)
//{
//	gamepad = SDL_GameControllerOpen(device);
//	SDL_Joystick* j = SDL_GameControllerGetJoystick(gamepad);
//	instance_id = SDL_JoystickInstanceID(j);
//	is_connected = true;
//	if (SDL_JoystickIsHaptic(j)) {
//		haptic = SDL_HapticOpenFromJoystick(j);
//		if (SDL_HapticRumbleSupported(haptic)) {
//			if (SDL_HapticRumbleInit(haptic) != 0) {
//				SDL_HapticClose(haptic);
//				haptic = 0;
//			}
//		}
//		else {
//			SDL_HapticClose(haptic);
//			haptic = 0;
//		}
//	}
//}
//
//void JoypadController::Close()
//{
//	if (is_connected) {
//		is_connected = false;
//		if (haptic) {
//			SDL_HapticClose(haptic);
//			haptic = 0;
//		}
//		SDL_GameControllerClose(gamepad);
//		gamepad = 0;
//	}
//}
//
//int JoypadController::GetControllerIndex(SDL_JoystickID instance)
//{
//	for (int i = 0; i < MAX_CONTROLLERS; ++i)
//	{
//		if (ms_controllers[i].is_connected && ms_controllers[i].instance_id == instance) {
//			return i;
//		}
//	}
//	return -1;
//}
//
//int JoypadController::processEvent(const SDL_Event& event)
//{
//	switch (event.type) {
//	case SDL_CONTROLLERAXISMOTION: {
//		// handle axis motion
//		break;
//	}
//	case SDL_CONTROLLERBUTTONDOWN:
//	case SDL_CONTROLLERBUTTONUP: {
//		// handle button up/down
//		break;
//	}
//	case SDL_CONTROLLERDEVICEADDED: {
//		if (event.cdevice.which < MAX_CONTROLLERS) {
//			JoypadController& jc = ms_controllers[event.cdevice.which];
//			jc.Open(event.cdevice.which);
//		}
//		break;
//	}
//	case SDL_CONTROLLERDEVICEREMOVED: {
//		int cIndex = GetControllerIndex(event.cdevice.which);
//		if (cIndex < 0) return 0; // unknown controller?
//		JoypadController& jc = ms_controllers[cIndex];
//		jc.Close();
//		break;
//	}
//	}
//	return 0;
//}

void GameBox::calculateStep()
{

	if (nextState != state)
	{
		resetGameBox();
		initialAttempt = true;
	}

	state = nextState;

	switch (state)
	{
	case GBStateMainmenu:
		mainMenu();
		break;
	case GBStatePong:
		pong();
	}

	initialAttempt = false;
}

void GameBox::returnToMainMenu()
{
		nextState = GBStateMainmenu;
}

void GameBox::resetGameBox()
{
	char i;

	objects.resetObjectCollection();

	for (i=0;i<GAMEBOX_FLOATS_NUMBER;i++)
	{
		floats[i] = 0;
	}

	for (i = 0; i < GAMEBOX_OBJECT_ID_NUMBER; i++)
	{
		objectIDs[i] = {0,OTypeNone};
	}

	for (i = 0; i < GAMEBOX_BOOLS_NUMBER; i++)
	{
		bools[i] = false;
	}

	for (i = 0; i < GAMEBOX_INTEGERS_NUMBER; i++)
	{
		integers[i] = 0;
	}
}

void GameBox::mainMenu()
{
	/*   REFERENCE GAME VARIABLES TO GAMEBOX VARIABLE SPACE    */
	tsObjectID &title = objectIDs[0], &underline = objectIDs[1], &rightBorder = objectIDs[2] ,&pongText = objectIDs[3],&bricksText = objectIDs[4];
	int &timer = integers[0],&currentSelection = integers[1];

	/*   GAME BEHAVIOR   */

	//Initialise game
	if (initialAttempt)
	{
		title = objects.addObject(Text({ 0,0 }, "MENU", { 10,10,10 }));
		underline = objects.addObject(Rectangle({ 8,4 }, 1, 16,{0,20,0}));
		rightBorder = objects.addObject(Rectangle({ 12,8 }, 16, 1, { 0,0,0 }));
		objects.deactivate(rightBorder);

		pongText = objects.addObject(Text({ 0,6 }, "PONG", {5,15,0}));
		bricksText = objects.addObject(Text({ 0,11 }, "BRICKS", { 5,15,0 }));

		objects.setVelocity(pongText,{-0.05,0});
		objects.setVelocity(bricksText, { -0.05,0 });

		currentSelection = 1;
		timer = 0;
	}	
	//Common behavior
	else
	{
		switch(currentSelection)
		{
			case 0:
				objects.setColor(pongText, {15,5,0});
				objects.motionStep(pongText);

				if (!objects.checkCollision(rightBorder, pongText))
				{
					objects.setPosition(pongText, { 5,6 });
				}
			case 1:
				objects.setColor(bricksText, { 15,5,0 });
				objects.motionStep(bricksText);

				if (!objects.checkCollision(rightBorder, bricksText))
				{
					objects.setPosition(bricksText, { 5,11 });
				}
			break;

		}








	}

}


	//if (!Actors[0])
	//{
	//	id1 = objects.addObject(Rectangle({ 10,10 }, 4, 5, { 15,0,10 }));
	//	id2 = objects.addObject(Rectangle({ 8,8 }, 3, 3, { 5,20,0 }));
	//	//id2 = objects.addObject(Circle({ 4,4 }, 3,{ 10,5,0}));
	//	//id4 = objects.addObject(Text({ 0,0 },"P1",{ 15,5,1 }));
	//	//id5 = objects.addObject(Text({ 9,0 }, "P2", { 5,15,1 }));

	//	id6 = objects.addObject(Text({ 0,0 }, "COLL", { 0,0,0 }));

	//	Actors[0] = 1;
	//    Actors[1] = 1;
	//	Actors[2] = 1;

	//}

	//if (objects.checkCollision(id1, id2))
	//{
	//	objects.setColor(id6, { 30,0,0 });



	//}
	//else
	//{
	//	objects.setColor(id6, { 0,30,0 });
	//}

	//objects.setPosition(id1, { objects.getPosition(id1).x + Actors[1]/5,objects.getPosition(id1).y});
	//objects.setPosition(id2, { objects.getPosition(id2).x,objects.getPosition(id2).y + Actors[2] / 3.25 });


	//if (objects.getPosition(id1).x > 18)
	//{
	//	Actors[1] = -1;
	//}
	//else if(objects.getPosition(id1).x < -5)
	//{
	//	Actors[1] = 1;
	//}

	//if (objects.getPosition(id2).y > 18)
	//{
	//	Actors[2] = -1;
	//}
	//else if (objects.getPosition(id2).y < -5)
	//{
	//	Actors[2] = 1;
	//}

