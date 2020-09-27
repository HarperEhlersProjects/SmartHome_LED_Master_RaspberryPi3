#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H
#pragma once

#include "gamepad.h"

#include "../../color.h"
#include "../../display.h"
#include <vector>
#include <string>
#include <cstdint>

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

namespace game_objects
{
	class GameObject
	{
	public:

		GameObject() :
			type(OTypeNone), position({ 0,0 }), velocity({ 0,0 }), acceleration({ 0,0 }), color({ 0,0,0 }), is_active(false) {}

		GameObject(ObjectType type, bool is_active, tsPosition position, tsRGB color) :
			type(type), is_active(is_active), position(position), velocity({ 0,0 }), acceleration({ 0,0 }), color(color) {}

		ObjectType getType(void);

		void setPosition(tsPosition position);
		tsPosition getPosition(void);

		void setVelocity(tsVelocity velocity);
		tsVelocity getVelocity(void);

		void setAcceleration(tsAcceleration acceleration);
		tsAcceleration getAcceleration(void);

		void setColor(tsRGB color);
		tsRGB getColor(void);

		//Change visibility and interactability of object
		void activate(void);
		void deactivate(void);
		bool isActive(void);

		//Calculate the kinetics of one time step
		void move(void);

		virtual void draw(DPU *display) = 0;

	private:
		ObjectType type;

		tsPosition position;
		tsVelocity velocity;
		tsAcceleration acceleration;

		tsRGB color;

		bool is_active;
		bool removed = false;
	};

	//Textobject which is defined by gameobjects properties and a string.
	class Text : virtual public GameObject
	{
	public:

		std::string string;

		Text():
			GameObject(OTypeText, false, { 0,0 }, {0,0,0}) {}

		Text(tsPosition position, std::string string, tsRGB color) :
			string(string), GameObject(OTypeText, true, position, color) {}

		void draw(DPU *display) override;
	};

	//Rectangle which is defined by gameobjects properties, height and length.
	class Rectangle : public GameObject
	{
	public:
		float height;
		float length;

		Rectangle() :
			GameObject(OTypeRectangle, false, { 0,0 }, { 0,0,0 }) {}

		Rectangle(tsPosition position, char height, char length, tsRGB color) :
			height(height), length(length), GameObject(OTypeRectangle, true, position, color) {}

		void draw(DPU *display) override;
	};

	//Circle which is defined by gameobjects properties and radius
	class Circle : public GameObject
	{
	public:

		float radius;

		Circle() :
			GameObject(OTypeCircle, false, { 0,0 }, { 0,0,0 }) {}

		Circle(tsPosition position, char radius, tsRGB color) :
			radius(radius), GameObject(OTypeCircle, true, position, color) {}

		void draw(DPU *display) override;
	};

	//Triangle which is defined by a baseLength and angulars of 45°,45° and 90°.
	class Triangle : public GameObject
	{
	public:

		float baseLength;

		Triangle() : 
			GameObject(OTypeTriangle, false, { 0,0 }, { 0,0,0 }) {}

		Triangle(tsPosition position, char baseLength, tsRGB color) :
			baseLength(baseLength), GameObject(OTypeTriangle, true, position, color) {}

		void draw(DPU* display) override;
	};

}

#endif // !GAME_OBJECTS_H