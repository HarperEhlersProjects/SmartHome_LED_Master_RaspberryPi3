#include "game_box.h"

#include <iostream>


void GameBox::calculateStep()
{
	gamepads.updateControllers();

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

