#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#pragma once

#include "color.h"
#include "display.h"

typedef struct {
	float x;
	float y;
} tsPosition, tsVelocity, tsAcceleration;


enum ObjectType
{
	OTypeNone,
	OTypeText,
	OTypeRectangle,
	OTypeCircle,
	OTypeTriangle
};

class Text
{
public:

	tsPosition position;
	tsVelocity velocity;
	tsAcceleration acceleration;
	char string[50];
	char stringLength;

	tsRGB color;

	char idnumber;

	bool isActive = true;

	Text()
	{
	}

	Text(tsPosition position, char* string, tsRGB color);
};

class Rectangle
{
public:
	tsPosition position;
	tsVelocity velocity;
	tsAcceleration acceleration;
	float height;
	float length;

	tsRGB color;

	bool isActive;

	char idnumber;

	Rectangle(tsPosition position, char height, char length, tsRGB color)
	{
		this->position = position;
		this->velocity = { 0,0 };
		this->acceleration = { 0,0 };
		this->height = height;
		this->length = length;

		this->color = color;

		this->isActive = true;
	}
};

class Circle
{
public:
	tsPosition position;
	tsVelocity velocity;
	tsAcceleration acceleration;
	float radius;

	tsRGB color;

	bool isActive;

	char idnumber;

	Circle(tsPosition position, char radius, tsRGB color)
	{
		this->position = position;
		this->velocity = { 0,0 };
		this->acceleration = { 0,0 };
		this->radius = radius;
		this->color = color;

		this->isActive = true;
	}
};


class Triangle
{
public:
	tsPosition position;
	tsVelocity velocity;
	tsAcceleration acceleration;
	float baseLength;

	tsRGB color;

	bool isActive;

	int idnumber;

	Triangle(tsPosition position, char baseLength, tsRGB color)
	{
		this->position = position;
		this->velocity = { 0,0 };
		this->acceleration = { 0,0 };
		this->baseLength = baseLength;
		this->color = color;

		this->isActive = true;
	}
};

typedef struct {
	int id;
	ObjectType type;
}tsObjectID;

class ObjectCollection
{
public:

	Text* texts;
	Rectangle* rectangles;
	Circle* circles;
	Triangle* triangles;

	int numberOfElementsText;
	int numberOfElementRectangle;
	int numberOfElementsCircle;
	int numberOfElementsTriangle;

	ObjectCollection()
	{
		texts = nullptr;
		rectangles = nullptr;
		circles = nullptr;
		triangles = nullptr;

		numberOfElementsText = 0;
		numberOfElementRectangle = 0;
		numberOfElementsCircle = 0;
		numberOfElementsTriangle = 0;
	}

	tsObjectID addObject(Text text);
	tsObjectID addObject(Rectangle rectangle);
	tsObjectID addObject(Circle circle);
	tsObjectID addObject(Triangle triangle);

	void setPosition(tsObjectID id, tsPosition position);
	tsPosition getPosition(tsObjectID id);
	void setVelocity(tsObjectID id, tsVelocity velocity);
	tsVelocity getVelocity(tsObjectID id);
	void setAcceleration(tsObjectID id, tsAcceleration acceleration);
	tsAcceleration getAcceleration(tsObjectID id);
	void motionStep(tsObjectID id);
	void motionStep(void);
	void setColor(tsObjectID id, tsRGB color);
	void deactivate(tsObjectID object);
	void activate(tsObjectID object);

	bool checkCollision(Rectangle rectangle1, Rectangle rectangle2);
	bool checkCollision(Circle circle1, Circle circle2);
	bool checkCollision(Rectangle rectangle, Circle circle);
	bool checkCollision(tsObjectID object1, tsObjectID object2);

	void removeObject(tsObjectID id);
	int getIndex(tsObjectID id);
	void resetObjectCollection();

	void drawObjects(DPU* display);

	void drawObject(DPU* display, Text text);
	void drawObject(DPU* display, Rectangle rectangle);
	void drawObject(DPU* display, Circle circle);
	void drawObject(DPU* display, Triangle triangle);

	bool isInsideBorders(DPU* display, tsPosition coord);
};







#endif // !GAME_OBJECTS_H