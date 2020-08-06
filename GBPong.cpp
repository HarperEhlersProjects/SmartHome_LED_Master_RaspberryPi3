#include "game_box.h"

void GameBox::pong()
{
/*   REFERENCE GAME VARIABLES TO GAMEBOX VARIABLE SPACE    */
	tsObjectID &ball = objectIDs[0], &wallS = objectIDs[1], & wallN = objectIDs[2], & wallE = objectIDs[3], & wallW = objectIDs[4];


/*   GAME BEHAVIOR   */

	//Initialise game
	if (initialAttempt)
	{
		ball = objects.addObject(Rectangle({ 3,2 }, 2, 2, {10,10,10}));
		wallS = objects.addObject(Rectangle({8,15 }, 1, 20, { 0,10,0 }));
		wallN = objects.addObject(Rectangle({ 8,0}, 1, 20, { 0,10,0 }));
		wallE = objects.addObject(Rectangle({ 15,8}, 20, 1, { 0,10,0 }));
		wallW = objects.addObject(Rectangle({ 0,8 }, 20, 1, { 0,10,0 }));
		objects.setVelocity(ball,{0.3,0.5});
		//objects.deactivate(wallS);

	}
	//Common behavior
	else
	{
		objects.motionStep(ball);

		if (objects.checkCollision(ball, wallS) || objects.checkCollision(ball, wallN))
		{
			objects.setVelocity(ball,{objects.getVelocity(ball).x,-objects.getVelocity(ball).y });
		}
		if (objects.checkCollision(ball, wallE) || objects.checkCollision(ball, wallW))
		{
			objects.setVelocity(ball, { -objects.getVelocity(ball).x,objects.getVelocity(ball).y });
		}

	}

}