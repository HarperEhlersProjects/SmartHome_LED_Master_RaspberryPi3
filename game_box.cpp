#include "game_box.h"

#include <iostream>

using namespace gameobjects;

void GameBox::calculateStep()
{
	//Update controller inputs
	gamepads.updateControllers();

	//In case of state switching reset the objectcollection and signal the initial attempt of next state.
	if (nextState != state)
	{
		resetGameBox();
		initialAttempt = true;
	}

	state = nextState;

	//Multiplex the current state of game box.
	switch (state)
	{
	case GBStateMainmenu:
		mainMenu();
		break;
	case GBStatePong:
		pong();
		break;
	}

	initialAttempt = false;
}

//Change the next state to main menu.
void GameBox::returnToMainMenu()
{
	nextState = GBStateMainmenu;
}

//Clear the whole memory of game objects and variables.
void GameBox::resetGameBox()
{
	char i;

	objects.resetObjectCollection();

	for (i=0;i<GAMEBOX_FLOATS_NUMBER;i++)
	{
		floats[i] = 0;
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
	/*   Allocate memory and refer pointers to game variables    */
	objects.texts.resize(3);
	objects.rectangles.resize(2);

	Text &title = objects.texts[0], &pongText = objects.texts[1], &bricksText = objects.texts[2];
	Rectangle &underline = objects.rectangles[0], &rightBorder = objects.rectangles[1];
	int &currentSelection = integers[0];
	bool &buttonUP = bools[0], &buttonDOWN = bools[1];

	/*   GAME BEHAVIOR   */

	//Initialise game
	if (initialAttempt)
	{
		//Background
		title = Text({ 0,0 }, "MENU", { 10,10,10 });
		underline = Rectangle({ 8,4 }, 1, 16,{0,20,0});

		//Invisible wall for animation event
		rightBorder = Rectangle({ 12,8 }, 16, 1, { 0,0,0 });
		rightBorder.deactivate();

		//Gametexts
		pongText = Text({ 0,6 }, "PONG", {5,15,0});
		bricksText = Text({ 0,11 }, "BRICKS", {5,15,0 });

		//Velocity in case of selection
		pongText.setVelocity({ -0.05,0 });
		bricksText.setVelocity({ -0.05,0 });

		//Variables for checking rising edges
		buttonUP = false;
		buttonDOWN = true;

		//Selected game
		currentSelection = 1;
	}	
	//Common behavior
	else
	{
		//Check for rising edge of DOWN button and change selection downwards
		if ((gamepads.inputs[0].DOWN || gamepads.inputs[1].DOWN) && !buttonDOWN)
		{
			buttonDOWN = true;
			switch (currentSelection)
			{
			case 0:
				pongText.setPosition({ 0,6 });
				pongText.setColor({ 5,15,0 });
				currentSelection = 1;
				break;
			case 1:
				bricksText.setPosition({ 0,11 });
				bricksText.setColor({ 5,15,0 });
				currentSelection = 0;
				break;
			default:
				break;
			}
		}
		else if(!(gamepads.inputs[0].DOWN || gamepads.inputs[1].DOWN))
		{
			buttonDOWN = false;
		}

		//Check for rising edge of UP button and change selection upwards
		if ((gamepads.inputs[0].UP || gamepads.inputs[1].UP) && !buttonUP)
		{
			buttonUP = true;
			switch (currentSelection)
			{
			case 0:
				pongText.setPosition({ 0,6 });
				pongText.setColor({ 5,15,0 });
				currentSelection = 1;
				break;
			case 1:
				bricksText.setPosition({ 0,11 });
				bricksText.setColor({ 5,15,0 });
				currentSelection = 0;
				break;
			default:
				break;
			}
		}
		else if(!(gamepads.inputs[0].UP || gamepads.inputs[1].UP))
		{
			buttonUP = false;
		}

		//Start selected game when A is pushed
		if (gamepads.inputs[0].A || gamepads.inputs[1].A)
		{
			switch (currentSelection)
			{
			case 0:
				nextState = GBStatePong;
				break;
			case 1:

				break;
			default:
				break;
			}
		}

		//Animation of selected game text
		switch(currentSelection)
		{
			case 0:
				pongText.setColor({ 15,5,0 });
				pongText.move();

				if (!objects.checkCollision(pongText, rightBorder))
				{
					pongText.setPosition({ 5,6 });
				}
			break;
			case 1:
				bricksText.setColor({ 15,5,0 });
				bricksText.move();

				if (!objects.checkCollision(bricksText,rightBorder))
				{
					bricksText.setPosition({ 5,11 });
				}
			break;
		}
	}
}