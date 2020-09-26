#include "game_box.h"
#include "stdio.h"

void GameBox::pong()
{
/*    Allocate memory and refer pointers to game variables    */
	objects.rectangles.resize(10);
	Rectangle &ball = objects.rectangles[0], &wallS = objects.rectangles[1], &wallN = objects.rectangles[2], &wallE = objects.rectangles[3], &wallW = objects.rectangles[4], &player1 = objects.rectangles[5], &player2 = objects.rectangles[6];

/*   GAME BEHAVIOR   */

	//Initialise game
	if (initialAttempt)
	{
		ball = Rectangle({ 7,7 }, 2, 2, {10,10,10});
		wallS = Rectangle({8,15 }, 1, 20, { 0,10,0 });
		wallN = Rectangle({ 8,0}, 1, 20, { 0,10,0 });
		wallE = Rectangle({ 20,8}, 40, 1, { 0,0,0 });
		wallW = Rectangle({ -5,8 }, 40, 1, { 0,0,0 });
		player1 = Rectangle({0,8}, 3, 1, { 10,0,0 });
		player2 = Rectangle({15,8}, 3, 1, { 0,0,10 });

		ball.setVelocity({ 0.3,-0.2 });
	}
	//Common behavior
	else
	{
		if (gamepads.inputs[0].SELECT || gamepads.inputs[1].SELECT)
		{
			returnToMainMenu();
		}

		//Initial velocity of ball
		player1.setVelocity({ 0,((int)gamepads.inputs[0].DOWN) * 0.2 - ((int)gamepads.inputs[0].UP) * 0.2 });
		player2.setVelocity({ 0,((int)gamepads.inputs[1].DOWN) * 0.2 - ((int)gamepads.inputs[1].UP) * 0.2 });

		//calculate one time step
		objects.move();

		//Ball gets reflected from top or bottom wall
		if (objects.checkCollision(ball, wallS) || objects.checkCollision(ball, wallN))
		{
			ball.setVelocity({ ball.getVelocity().x,-ball.getVelocity().y });
		}

		//Ball reaches left or right goal and game restarts
		if (objects.checkCollision(ball, wallW) || objects.checkCollision(ball, wallE))
		{
			ball.setPosition({ 7,7 });
			ball.setVelocity({ 0.3,-0.2 });
		}

		//Ball get reflected by one player
		if (objects.checkCollision(ball, player1) || objects.checkCollision(ball, player2))
		{
			ball.setVelocity({ -ball.getVelocity().x,ball.getVelocity().y });
		}

	}

}