#include "GBPong.h"



GBPong::GBPong(GamePads* gamepads)
{
	nextState = GTypePong;
	this->gamepads = gamepads;

	ball = Rectangle({ 7,7 }, 2, 2, { 80,80,80 });
	wallS = Rectangle({ 8,15 }, 1, 20, { 0,90,0 });
	wallN = Rectangle({ 8,0 }, 1, 20, { 0,90,0 });
	wallE = Rectangle({ 20,8 }, 40, 1, { 0,0,0 });
	wallW = Rectangle({ -5,8 }, 40, 1, { 0,0,0 });
	player1 = Rectangle({ 0,8 }, 3, 1, { 80,0,0 });
	player2 = Rectangle({ 15,8 }, 3, 1, { 0,0,80 });

	registerObject(&ball);
	registerObject(&wallS);
	registerObject(&wallN);
	registerObject(&wallE);
	registerObject(&wallW);
	registerObject(&player1);
	registerObject(&player2);

	ball.setVelocity({ 0.3,-0.2 });
}

void GBPong::calculateStep(void)
{
	if (gamepads->inputs[0].SELECT || gamepads->inputs[1].SELECT)
	{
		returnToMainMenu();
	}

	//Initial velocity of ball
	player1.setVelocity({ 0,((int)gamepads->inputs[0].DOWN) * 0.2 - ((int)gamepads->inputs[0].UP) * 0.2 });
	player2.setVelocity({ 0,((int)gamepads->inputs[1].DOWN) * 0.2 - ((int)gamepads->inputs[1].UP) * 0.2 });

	//calculate one time step
	move();

	//Ball gets reflected from top or bottom wall
	if (checkCollision(ball, wallS) || checkCollision(ball, wallN))
	{
		ball.setVelocity({ ball.getVelocity().x,-ball.getVelocity().y });
	}

	//Ball reaches left or right goal and game restarts
	if (checkCollision(ball, wallW) || checkCollision(ball, wallE))
	{
		ball.setPosition({ 7,7 });
		ball.setVelocity({ 0.3,-0.2 });
	}

	//Ball get reflected by one player
	if (checkCollision(ball, player1) || checkCollision(ball, player2))
	{
		ball.setVelocity({ -ball.getVelocity().x,ball.getVelocity().y });
	}

}