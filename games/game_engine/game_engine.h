#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#pragma once

#include "game_objects.h"

using namespace game_objects;

enum GameType {
	GTypeNone,
	GTypeInit,
	GTypeMainmenu,
	GTypePong,
	GTypeBricks,
	GTypeSnake
};

class Game
{
public:

	Game() : nextState(GTypeNone), gamepads(GamePads()) {}
	Game(GameType nextState) : nextState(nextState), gamepads(GamePads()) {}

	virtual ~Game() {}

	GamePads gamepads;
	GameType nextState;

	void move(void);

	bool checkCollision(Rectangle rectangle1, Rectangle rectangle2);
	bool checkCollision(Circle circle1, Circle circle2);
	bool checkCollision(Rectangle rectangle, Circle circle);
	bool checkCollision(Circle circle, Rectangle rectangle);
	bool checkCollision(Text text, Rectangle rectangle);
	bool checkCollision(Rectangle rectangle, Text text);

	void registerObject(GameObject* object);

	//template <typename T>
	//T* registerObject(T object);

	void drawObjects(DPU* display);

	void returnToMainMenu();

	virtual void calculateStep(void) = 0;


private:

	std::vector<GameObject*> objects;
};

#endif // !GAME_ENGINE_H