#include "game_engine.h"
#include <cmath>


//Calculate the kinetics of all objects for one timestep.
void Game::move()
{
	for (uint16_t i = 0; i < objects.size(); i++)
	{
		objects[i]->move();
	}
}

void Game::returnToMainMenu()
{
	nextState = GTypeMainmenu;
}

void Game::registerObject(GameObject* object)
{
	objects.push_back(object);
}

//template <typename T>
//T* Game::registerObject(T object)
//{
//	T* new_object = new T();
//	*new_object = object;
//
//	objects.push_back(new_object);
//
//	return new_object;
//}

bool Game::checkCollision(Rectangle rectangle1, Rectangle rectangle2)
{
	return ((fabs(rectangle1.getPosition().x - rectangle2.getPosition().x) <= (rectangle1.length + rectangle2.length) / 2) && (fabs(rectangle1.getPosition().y - rectangle2.getPosition().y) <= (rectangle1.height + rectangle2.height) / 2));
}

bool Game::checkCollision(Circle circle1, Circle circle2)
{
	return (sqrt((circle1.getPosition().x - circle2.getPosition().x) * (circle1.getPosition().x - circle2.getPosition().x) + (circle1.getPosition().y - circle2.getPosition().y) * (circle1.getPosition().y - circle2.getPosition().y)) <= (circle1.radius + circle2.radius));
}

//Check collision of a rectangle and a circle
bool Game::checkCollision(Rectangle rectangle, Circle circle)
{
	float XLeft = rectangle.getPosition().x - rectangle.length / 2, XRight = rectangle.getPosition().x + rectangle.length / 2, YTop = rectangle.getPosition().y - rectangle.height / 2, YBot = rectangle.getPosition().y + rectangle.height / 2;
	float XLeftEdit = XLeft - circle.getPosition().x, XRightEdit = XRight - circle.getPosition().x, YTOPEdit = YTop - circle.getPosition().y, YBotEdit = YBot - circle.getPosition().y;
	float XLeftEditSquare = XLeftEdit * XLeftEdit, XRightEditSquare = XRightEdit * XRightEdit, YTOPEditSquare = YTOPEdit * YTOPEdit, YBotEditSquare = YBotEdit * YBotEdit;

	bool CircleIsVerticalyInbetween = (circle.getPosition().y >= YTop && circle.getPosition().y <= YBot), CircleIsHorizontalyInbetween = (circle.getPosition().x >= XLeft && circle.getPosition().x <= XRight);

	return (CircleIsHorizontalyInbetween <= XRight && CircleIsVerticalyInbetween) ||
		(sqrt(XLeftEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XLeftEditSquare + YBotEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YBotEditSquare) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.getPosition().y >= circle.getPosition().y && fabs(YTOPEdit) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.getPosition().y <= circle.getPosition().y && fabs(YBotEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.getPosition().x >= circle.getPosition().x && fabs(XLeftEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.getPosition().x <= circle.getPosition().x && fabs(XRightEdit) <= circle.radius);
}

//Check collision of a circle and a rectangle
bool Game::checkCollision(Circle circle, Rectangle rectangle)
{
	float XLeft = rectangle.getPosition().x - rectangle.length / 2, XRight = rectangle.getPosition().x + rectangle.length / 2, YTop = rectangle.getPosition().y - rectangle.height / 2, YBot = rectangle.getPosition().y + rectangle.height / 2;
	float XLeftEdit = XLeft - circle.getPosition().x, XRightEdit = XRight - circle.getPosition().x, YTOPEdit = YTop - circle.getPosition().y, YBotEdit = YBot - circle.getPosition().y;
	float XLeftEditSquare = XLeftEdit * XLeftEdit, XRightEditSquare = XRightEdit * XRightEdit, YTOPEditSquare = YTOPEdit * YTOPEdit, YBotEditSquare = YBotEdit * YBotEdit;

	bool CircleIsVerticalyInbetween = (circle.getPosition().y >= YTop && circle.getPosition().y <= YBot), CircleIsHorizontalyInbetween = (circle.getPosition().x >= XLeft && circle.getPosition().x <= XRight);

	return (CircleIsHorizontalyInbetween <= XRight && CircleIsVerticalyInbetween) ||
		(sqrt(XLeftEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XLeftEditSquare + YBotEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YBotEditSquare) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.getPosition().y >= circle.getPosition().y && fabs(YTOPEdit) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.getPosition().y <= circle.getPosition().y && fabs(YBotEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.getPosition().x >= circle.getPosition().x && fabs(XLeftEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.getPosition().x <= circle.getPosition().x && fabs(XRightEdit) <= circle.radius);
}

bool Game::checkCollision(Text text, Rectangle rectangle)
{
	checkCollision(Rectangle({ text.getPosition().x + 4 * text.string.length() / 2,text.getPosition().y }, 4, 4 * text.string.length(), { 0,0,0 }), rectangle);
}

bool Game::checkCollision(Rectangle rectangle, Text text)
{
	checkCollision(Rectangle({ text.getPosition().x + 4 * text.string.length() / 2,text.getPosition().y }, 4, 4 * text.string.length(), { 0,0,0 }), rectangle);
}

void Game::drawObjects(DPU* display)
{
	for (uint16_t i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(display);
	}
}