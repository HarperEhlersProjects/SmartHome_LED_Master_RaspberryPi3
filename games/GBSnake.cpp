#include "GBSnake.h"
#include <cmath>
#include <cstdint>
#include <time.h>

GBSnake::GBSnake(GamePads* gamepads)
{
	nextState = GTypeSnake;
	this->gamepads = gamepads;

	srand((unsigned)time(NULL));

	//for (uint16_t i = 0; i < 16; i++)
	//{
	//	for (uint16_t j = 0; j < 16; j++)
	//	{
	//		grid[i][j] = Rectangle({ i,j }, 1, 1, { 0,0,0});
	//		registerObject(&grid[i][j]);
	//	}
	//}

	fruit = Rectangle({ 7,7 }, 1, 1, { 10,0,0 });
	registerObject(&fruit);

	//Initialize snake
	snake.reserve(16*16);
	snake.push_back(Rectangle({ 5,0 }, 1, 1, { 0,10,10 }));
	snake.push_back(Rectangle({ 4,0 }, 1, 1, { 0,10,0 }));
	snake.push_back(Rectangle({ 3,0 }, 1, 1, { 0,10,0 }));
	snake.push_back(Rectangle({ 2,0 }, 1, 1, { 0,10,0 }));
	snake.push_back(Rectangle({ 1,0 }, 1, 1, { 0,10,0 }));
	snake.push_back(Rectangle({ 0,0 }, 1, 1, { 0,10,0 }));

	//Register snake
	for (uint16_t i = 0; i < snake.size(); i++)
	{
		registerObject(&snake[i]);
	}

	head = snake[0].getPosition();
	//Timer for movement
	timer = 0;

	buttonLeft = false;
	buttonRight = false;

	//Initial direction
	dir = RIGHT;
}

void GBSnake::calculateStep(void)
{
	//Return to main menu when SELECT is pushed
	if (gamepads->inputs[0].SELECT || gamepads->inputs[1].SELECT)
	{
		returnToMainMenu();
	}


	//Check for rising edge of LEFT button and turn left
	if ((gamepads->inputs[0].LEFT || gamepads->inputs[1].LEFT) && !buttonLeft)
	{
		buttonLeft = true;
		switch (dir)
		{
		case UP:	dir = LEFT;
			break;
		case DOWN:	dir = RIGHT;
			break;
		case LEFT:	dir = DOWN;
			break;
		case RIGHT:	dir = UP;
			break;
		}
	}
	else if (!(gamepads->inputs[0].LEFT || gamepads->inputs[1].LEFT))
	{
		buttonLeft = false;
	}

	//Check for rising edge of RIGHT button and turn right
	if ((gamepads->inputs[0].RIGHT || gamepads->inputs[1].RIGHT) && !buttonRight)
	{
		buttonRight = true;
		switch (dir)
		{
		case UP:	dir = RIGHT;
			break;
		case DOWN:	dir = LEFT;
			break;
		case LEFT:	dir = UP;
			break;
		case RIGHT:	dir = DOWN;
			break;
		}
	}
	else if (!(gamepads->inputs[0].RIGHT || gamepads->inputs[1].RIGHT))
	{
		buttonRight = false;
	}

	timer++;
	if (timer > 8)
	{
		//Preserve tail coordinate
		temp = snake[snake.size()-1].getPosition();
		//Move the snake
		for (uint16_t i = snake.size()-1; i > 0; i--)
		{
			snake[i].setPosition({ snake[i-1].getPosition().x ,snake[i-1].getPosition().y });
		}

		//Move head of snake
		switch (dir)
		{
		case UP:	snake[0].setPosition({ snake[0].getPosition().x,snake[0].getPosition().y - 1 });
			break;
		case DOWN:	snake[0].setPosition({ snake[0].getPosition().x,snake[0].getPosition().y + 1 });
			break;
		case LEFT:	snake[0].setPosition({ snake[0].getPosition().x - 1,snake[0].getPosition().y });
			break;
		case RIGHT:	snake[0].setPosition({ snake[0].getPosition().x + 1,snake[0].getPosition().y });
			break;
		}
		head = snake[0].getPosition();
		timer = 0;

		//When snake eats fruit
		if (head.x == fruit.getPosition().x && head.y == fruit.getPosition().y)
		{
			//Add one element to snake
			snake.push_back(Rectangle(temp, 1, 1, { 0,10,0 }));
			registerObject(&snake[snake.size() - 1]);

			//Search for new available random position for fruit
			do 
			{
				fruitOnSnake = false;
				temp = {(rand() % 15)+1,(rand() % 15) + 1 };

				for (uint16_t i = 0; i < snake.size(); i++)
				{
					if (snake[i].getPosition().x == temp.x && snake[i].getPosition().y == temp.y)
					{
						fruitOnSnake = true;
					}
				}

			} while (fruitOnSnake);
			fruit.setPosition(temp);
		}
	}

	//Condition for game freeze (Snake eats itself)
	for (uint16_t i = snake.size() - 1; i > 0; i--)
	{
		if (head.x == snake[i].getPosition().x && head.y == snake[i].getPosition().y)
		{
			timer = 0;
		}
	}
}