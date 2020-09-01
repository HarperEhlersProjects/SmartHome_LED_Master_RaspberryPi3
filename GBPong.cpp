#include "game_box.h"

void GameBox::pong()
{
/*   REFERENCE GAME VARIABLES TO GAMEBOX VARIABLE SPACE    */
	tsObjectID &ball = objectIDs[0], &wallS = objectIDs[1], &wallN = objectIDs[2], &wallE = objectIDs[3], &wallW = objectIDs[4], &player1 = objectIDs[5], &player2 = objectIDs[6];


/*   GAME BEHAVIOR   */

	//Initialise game
	if (initialAttempt)
	{
		ball = objects.addObject(Rectangle({ 7,7 }, 1, 1, {10,10,10}));
		wallS = objects.addObject(Rectangle({8,15 }, 1, 20, { 0,10,0 }));
		wallN = objects.addObject(Rectangle({ 8,0}, 1, 20, { 0,10,0 }));
		wallE = objects.addObject(Rectangle({ 20,8}, 40, 1, { 0,0,0 }));
		wallW = objects.addObject(Rectangle({ -5,8 }, 40, 1, { 0,0,0 }));
		player1 = objects.addObject(Rectangle({0,8}, 3, 1, { 10,0,0 }));
		player2 = objects.addObject(Rectangle({15,8}, 3, 1, { 0,0,10 }));

		objects.setVelocity(ball,{0.3,-0.2});
		//objects.setAcceleration(ball, { 0.00,0.1 });
		//objects.deactivate(wallS);

	}
	//Common behavior
	else
	{
		objects.setVelocity(player1, {0,((int)gamepads.inputs[0].DOWN) * 0.2 - ((int)gamepads.inputs[0].UP) * 0.2 });
		objects.setVelocity(player2, { 0,((int)gamepads.inputs[0].DOWN) * 0.2 - ((int)gamepads.inputs[0].UP) * 0.2 });

		objects.motionStep();

		if (objects.checkCollision(ball, wallS) || objects.checkCollision(ball, wallN))
		{
			objects.setVelocity(ball,{objects.getVelocity(ball).x,-objects.getVelocity(ball).y });
			//objects.setColor(ball, { 15,0,0 });
			//objects.setColor(wallN, { 6,6,6 });
			//objects.setColor(wallS, { 6,6,6 });
		}
		//else
		//{
		//	objects.setColor(wallN, { 0,10,0 });
		//	objects.setColor(wallS, { 0,10,0 });
		//}

		if (objects.checkCollision(ball, wallW) || objects.checkCollision(ball, wallE))
		{
			objects.setPosition(ball, { 7,7 });
			objects.setVelocity(ball, { 0.3,-0.2 });

			//objects.setVelocity(ball, { -objects.getVelocity(ball).x,objects.getVelocity(ball).y });
			//objects.setColor(ball, { 0,0,15 });
			//objects.setColor(wallE, { 6,6,6 });
			//objects.setColor(wallW, { 6,6,6 });
		}

		if (objects.checkCollision(ball, player1) || objects.checkCollision(ball, player2))
		{
			objects.setVelocity(ball, { -objects.getVelocity(ball).x,objects.getVelocity(ball).y });
		}


		//else
		//{
		//	objects.setColor(wallE, { 0,10,0 });
		//	objects.setColor(wallW, { 0,10,0 });
		//}

	}

}