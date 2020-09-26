#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#pragma once

#include "color.h"
#include "display.h"
#include <vector>
#include <string>

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

namespace gameobjects
{
	class GameObject
	{
	public:

		GameObject() :
			position({ 0,0 }), velocity({ 0,0 }), acceleration({ 0,0 }), color({ 0,0,0 }), is_active(true) {}

		GameObject(tsPosition position, tsRGB color) :
			position(position), velocity({ 0,0 }), acceleration({ 0,0 }), color(color), is_active(true) {}


		void setPosition(tsPosition position);
		tsPosition getPosition(void);

		void setVelocity(tsVelocity velocity);
		tsVelocity getVelocity(void);

		void setAcceleration(tsAcceleration acceleration);
		tsAcceleration getAcceleration(void);

		void setColor(tsRGB color);
		tsRGB getColor(void);

		void activate(void);
		void deactivate(void);
		bool isActive(void);

		void move(void);

		virtual void draw(DPU *display) = 0;

	private:
		tsPosition position;
		tsVelocity velocity;
		tsAcceleration acceleration;

		tsRGB color;

		bool is_active = true;
		bool removed = false;
	};

	//Textobject which is defined by gameobjects properties and a string.
	class Text : virtual public GameObject
	{
	public:

		std::string string;

		Text() {}

		Text(tsPosition position, std::string string, tsRGB color) :
			string(string), GameObject(position, color) {}

		void draw(DPU *display) override;
	};

	//Rectangle which is defined by gameobjects properties, height and length.
	class Rectangle : public GameObject
	{
	public:
		float height;
		float length;

		Rectangle() {};

		Rectangle(tsPosition position, char height, char length, tsRGB color) :
			height(height), length(length), GameObject(position, color) {}

		void draw(DPU *display) override;
	};

	//Circle which is defined by gameobjects properties and radius
	class Circle : public GameObject
	{
	public:

		float radius;

		Circle(tsPosition position, char radius, tsRGB color) :
			radius(radius), GameObject(position, color) {}

		void draw(DPU *display) override;
	};

	//Triangle which is defined by a baseLength and angulars of 45°,45° and 90°.
	class Triangle : public GameObject
	{
	public:

		float baseLength;

		Triangle(tsPosition position, char baseLength, tsRGB color) :
			baseLength(baseLength), GameObject(position, color) {}

		void draw(DPU* display) override;
	};


	class ObjectCollection
	{
	public:

		std::vector<Text> texts;
		std::vector<Rectangle> rectangles;
		std::vector<Circle> circles;
		std::vector<Triangle> triangles;

		//std::vector<GameObject> objects;

		ObjectCollection(){}

		void move(void);

		bool checkCollision(Rectangle rectangle1, Rectangle rectangle2);
		bool checkCollision(Circle circle1, Circle circle2);
		bool checkCollision(Rectangle rectangle, Circle circle);
		bool checkCollision(Circle circle, Rectangle rectangle);
		bool checkCollision(Text text, Rectangle rectangle);
		bool checkCollision(Rectangle rectangle, Text text);

		void resetObjectCollection();

		void drawObjects(DPU* display);
	};

}





#endif // !GAME_OBJECTS_H