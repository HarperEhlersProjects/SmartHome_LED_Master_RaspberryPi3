#include "GBMainMenu.h"

GBMainMenu::GBMainMenu(GamePads* gamepads)
{
	nextState = GTypeMainmenu;
	this->gamepads = gamepads;

	//Background
	title = Text({ 0,0 }, "MENU", tsRGB { 80,80,80 });
	underline = Rectangle({ 8,4 }, 1, 16, tsRGB {0,10,80});

	//Invisible wall for animation event
	rightBorder = Rectangle({ 12,8 }, 16, 1, { 0,0,0 });
	rightBorder.deactivate();

	//Gametexts
	pongText = Text({ 0,6 }, "PONG", tsRGB {30,110,0});
	snakeText = Text({ 0,11 }, "SNAKE", tsRGB {30,110,0 });

	//Velocity in case of selection
	pongText.setVelocity({ -0.05,0 });
	snakeText.setVelocity({ -0.05,0 });

	//Variables for checking rising edges
	buttonUP = false;
	buttonDOWN = true;

	//Selected game
	currentSelection = 0;

	registerObject(&title);
	registerObject(&underline);
	registerObject(&rightBorder);
	registerObject(&pongText);
	registerObject(&snakeText);
}

void GBMainMenu::calculateStep(void)
{
	//Common behavior
	
//Check for rising edge of DOWN button and change selection downwards
	if ((gamepads->inputs[0].DOWN || gamepads->inputs[1].DOWN) && !buttonDOWN)
	{
		buttonDOWN = true;
		switch (currentSelection)
		{
		case 0:
			pongText.setPosition({ 0,6 });
			pongText.setColor(tsRGB{ 30,110,0 });
			currentSelection = 1;
			break;
		case 1:
			snakeText.setPosition({ 0,11 });
			snakeText.setColor(tsRGB{ 30,110,0 });
			currentSelection = 0;
			break;
		default:
			break;
		}
	}
	else if (!(gamepads->inputs[0].DOWN || gamepads->inputs[1].DOWN))
	{
		buttonDOWN = false;
	}

	//Check for rising edge of UP button and change selection upwards
	if ((gamepads->inputs[0].UP || gamepads->inputs[1].UP) && !buttonUP)
	{
		buttonUP = true;
		switch (currentSelection)
		{
		case 0:
			pongText.setPosition({ 0,6 });
			pongText.setColor(tsRGB{ 30,110,0 });
			currentSelection = 1;
			break;
		case 1:
			snakeText.setPosition({ 0,11 });
			snakeText.setColor(tsRGB{ 30,110,0 });
			currentSelection = 0;
			break;
		default:
			break;
		}
	}
	else if (!(gamepads->inputs[0].UP || gamepads->inputs[1].UP))
	{
		buttonUP = false;
	}

	//Start selected game when A is pushed
	if (gamepads->inputs[0].A || gamepads->inputs[1].A)
	{
		switch (currentSelection)
		{
		case 0:
			nextState = GTypePong;
			break;
		case 1:
			nextState = GTypeSnake;
			break;
		default:
			break;
		}
	}

	//Animation of selected game text
	switch (currentSelection)
	{
	case 0:
		pongText.setColor(tsRGB{ 110,30,0 });
		pongText.move();

		if (!checkCollision(pongText, rightBorder))
		{
			pongText.setPosition({ 5,6 });
		}
		break;
	case 1:
		snakeText.setColor(tsRGB{ 110,30,0 });
		snakeText.move();

		if (!checkCollision(snakeText, rightBorder))
		{
			snakeText.setPosition({ 5,11 });
		}
		break;
	}

}