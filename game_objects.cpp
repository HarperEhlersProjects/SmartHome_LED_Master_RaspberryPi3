#include "game_objects.h"

#include "color.h"

#include <math.h>
#include <stdlib.h>
#include <cstdint>

//Definitions for 4x3 letters LUT
#define TEXT_0 {{false,true,false},{true,false,true},{true,false,true},{false,true,false}}
#define TEXT_1 {{true,true,false},{false,true,false},{false,true,false},{true,true,true}}
#define TEXT_2 {{true,true,false},{false,false,true},{false,true,false},{true,true,true}}
#define TEXT_3 {{true,true,false},{false,true,true},{false,false,true},{true,true,false}}
#define TEXT_4 {{true,false,true},{true,true,true},{false,false,true},{false,false,true}}
#define TEXT_5 {{true,true,true},{true,true,false},{false,false,true},{false,true,false}}
#define TEXT_6 {{false,true,true},{true,false,false},{true,true,true},{true,true,false}}
#define TEXT_7 {{true,true,true},{false,false,true},{false,true,false},{true,false,false}}
#define TEXT_8 {{true,true,true},{true,true,true},{true,false,true},{true,true,true}}
#define TEXT_9 {{false,true,true},{true,true,true},{false,false,true},{true,true,false}}

#define TEXT_A {{false,true,false},{true,false,true},{true,true,true},{true,false,true}}
#define TEXT_B {{true,true,true},{true,true,false},{true,false,true},{true,true,false}}
#define TEXT_C {{false,true,true},{true,false,false},{true,false,false},{false,true,true}}
#define TEXT_D {{true,true,false},{true,false,true},{true,false,true},{true,true,false}}
#define TEXT_E {{true,true,true},{true,true,false},{true,false,false},{true,true,true}}
#define TEXT_F {{true,true,true},{true,true,false},{true,false,false},{true,false,false}}
#define TEXT_G {{false,true,true},{true,false,false},{true,false,true},{false,true,true}}
#define TEXT_H {{true,false,true},{true,true,true},{true,false,true},{true,false,true}}
#define TEXT_I {{true,true,true},{false,true,false},{false,true,false},{true,true,true}}
#define TEXT_J {{true,true,true},{false,false,true},{true,false,true},{false,true,false}}
#define TEXT_K {{true,false,true},{true,true,false},{true,false,true},{true,false,true}}
#define TEXT_L {{true,false,false},{true,false,false},{true,false,false},{true,true,true}}
#define TEXT_M {{true,true,true},{true,true,true},{true,false,true},{true,false,true}}
#define TEXT_N {{true,true,false},{true,false,true},{true,false,true},{true,false,true}}
#define TEXT_O {{false,true,false},{true,false,true},{true,false,true},{false,true,false}}
#define TEXT_P {{true,true,true},{true,false,true},{true,true,true},{true,false,false}}
#define TEXT_Q {{false,true,false},{true,false,true},{true,true,true},{false,true,true}}
#define TEXT_R {{true,true,false},{true,false,true},{true,true,false},{true,false,true}}
#define TEXT_S {{true,true,true},{true,true,false},{false,false,true},{true,true,true}}
#define TEXT_T {{true,true,true},{false,true,false},{false,true,false},{false,true,false}}
#define TEXT_U {{true,false,true},{true,false,true},{true,false,true},{false,true,false}}
#define TEXT_V {{true,false,true},{true,false,true},{true,true,false},{true,false,false}}
#define TEXT_W {{true,false,true},{true,false,true},{true,true,true},{true,true,true}}
#define TEXT_X {{true,false,true},{false,true,false},{true,false,true},{true,false,true}}
#define TEXT_Y {{true,false,true},{false,true,false},{false,true,false},{false,true,false}}
#define TEXT_Z {{true,true,true},{false,true,false},{true,false,false},{true,true,true}}

#define TEXT_BLANK {{false,false,false},{false,false,false},{false,false,false},{false,false,false}}


//LUT for 3x4 letters
const bool Text4x3LUT[43][4][3] = { TEXT_0,TEXT_1,TEXT_2,TEXT_3,TEXT_4,TEXT_5,TEXT_6,TEXT_7,TEXT_8,TEXT_9,
									TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,
									TEXT_A,TEXT_B,TEXT_C,TEXT_D,TEXT_E,TEXT_F,TEXT_G,TEXT_H,TEXT_I,TEXT_J,
									TEXT_K,TEXT_L,TEXT_M,TEXT_N,TEXT_O,TEXT_P,TEXT_Q,TEXT_R,TEXT_S,TEXT_T,
									TEXT_U,TEXT_V,TEXT_W,TEXT_X,TEXT_Y,TEXT_Z };


Text::Text(tsPosition position, char* string, tsRGB color)
{
	char i = 0;

	while (string[i] != '\0' && i < 50)
	{
		this->string[i] = string[i];
		i++;
	}
	this->stringLength = i;

	this->isActive = true;
	this->position = position;
	this->velocity = { 0,0 };
	this->acceleration = { 0,0 };
	this->color = color;
}

int ObjectCollection::getIndex(tsObjectID id)
{
	char i, size;

	switch (id.type)
	{
	case OTypeText:

		size = numberOfElementsText;
		i = 0;
		while (i < size)
		{
			if (id.id == texts[i].idnumber)
			{
				return i;
			}
			i++;
		}
		return -1;
		break;
	case OTypeRectangle:

		size = numberOfElementRectangle;
		i = 0;
		while (i < size)
		{
			if (id.id == rectangles[i].idnumber)
			{
				return i;
			}
			i++;
		}
		return -1;
		break;
	case OTypeCircle:

		size = numberOfElementsCircle;
		i = 0;
		while (i < size)
		{
			if (id.id == circles[i].idnumber)
			{
				return i;
			}
			i++;
		}
		return -1;
		break;
	case OTypeTriangle:

		size = numberOfElementsTriangle;
		i = 0;
		while (i < size)
		{
			if (id.id == triangles[i].idnumber)
			{
				return i;
			}
			i++;
		}
		return -1;
		break;
	}

	return -1;
}


tsObjectID ObjectCollection::addObject(Text text)
{
	char newIndex = numberOfElementsText;
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeText }) != -1)
	{
		lowestAvailableID++;
	}

	this->texts = (Text*)realloc(this->texts, sizeof(Text) * (newIndex + 1));
	numberOfElementsText++;
	text.idnumber = lowestAvailableID;
	texts[newIndex] = text;

	return { lowestAvailableID,OTypeText };
}

tsObjectID ObjectCollection::addObject(Rectangle rectangle)
{
	char newIndex = numberOfElementRectangle;
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeRectangle }) != -1)
	{
		lowestAvailableID++;
	}

	this->rectangles = (Rectangle*)realloc(this->rectangles, sizeof(Rectangle) * (newIndex + 1));
	numberOfElementRectangle++;
	rectangle.idnumber = lowestAvailableID;
	rectangles[newIndex] = rectangle;

	return { lowestAvailableID,OTypeRectangle };
}

tsObjectID ObjectCollection::addObject(Circle circle)
{
	char newIndex = numberOfElementsCircle;
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeCircle }) != -1)
	{
		lowestAvailableID++;
	}

	this->circles = (Circle*)realloc(this->circles, sizeof(Circle) * (newIndex + 1));
	numberOfElementsCircle++;
	circle.idnumber = lowestAvailableID;
	circles[newIndex] = circle;

	return { lowestAvailableID,OTypeCircle };
}

tsObjectID ObjectCollection::addObject(Triangle triangle)
{
	char newIndex = numberOfElementsTriangle;
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeTriangle }) != -1)
	{
		lowestAvailableID++;
	}

	this->triangles = (Triangle*)realloc(this->triangles, sizeof(Triangle) * (newIndex + 1));
	numberOfElementsTriangle++;
	triangle.idnumber = lowestAvailableID;
	triangles[newIndex] = triangle;

	return { lowestAvailableID,OTypeTriangle };
}

void ObjectCollection::removeObject(tsObjectID id)
{
	char size;
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			size = numberOfElementsText;

			texts[index] = texts[size - 1];
			texts = (Text*)realloc(texts, sizeof(Text) * (size - 1));
			numberOfElementsText--;
			break;
		case OTypeRectangle:
			size = numberOfElementRectangle;

			rectangles[index] = rectangles[size - 1];
			rectangles = (Rectangle*)realloc(rectangles, sizeof(Rectangle) * (size - 1));
			numberOfElementRectangle--;
			break;
		case OTypeCircle:

			size = numberOfElementsCircle;

			circles[index] = circles[size - 1];
			circles = (Circle*)realloc(circles, sizeof(Circle) * (size - 1));
			numberOfElementsCircle--;
			break;
		case OTypeTriangle:

			size = numberOfElementsTriangle;

			triangles[index] = triangles[size - 1];
			triangles = (Triangle*)realloc(triangles, sizeof(Triangle) * (size - 1));
			break;
		}
	}
}

void ObjectCollection::setPosition(tsObjectID id, tsPosition position)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].position = position;
			break;
		case OTypeRectangle:
			rectangles[index].position = position;
			break;
		case OTypeCircle:
			circles[index].position = position;
			break;
		case OTypeTriangle:
			triangles[index].position = position;
			break;
		}
	}
}

tsPosition ObjectCollection::getPosition(tsObjectID id)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			return texts[index].position;
			break;
		case OTypeRectangle:
			return rectangles[index].position;
			break;
		case OTypeCircle:
			return circles[index].position;
			break;
		case OTypeTriangle:
			return triangles[index].position;
			break;
		}
	}
}

void ObjectCollection::setVelocity(tsObjectID id, tsVelocity velocity)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].velocity = velocity;
			break;
		case OTypeRectangle:
			rectangles[index].velocity = velocity;
			break;
		case OTypeCircle:
			circles[index].velocity = velocity;
			break;
		case OTypeTriangle:
			triangles[index].velocity = velocity;
			break;
		}
	}
}
tsVelocity ObjectCollection::getVelocity(tsObjectID id)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			return texts[index].velocity;
			break;
		case OTypeRectangle:
			return rectangles[index].velocity;
			break;
		case OTypeCircle:
			return circles[index].velocity;
			break;
		case OTypeTriangle:
			return triangles[index].velocity;
			break;
		}
	}
}
void ObjectCollection::setAcceleration(tsObjectID id, tsAcceleration acceleration)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].acceleration = acceleration;
			break;
		case OTypeRectangle:
			rectangles[index].acceleration = acceleration;
			break;
		case OTypeCircle:
			circles[index].acceleration = acceleration;
			break;
		case OTypeTriangle:
			triangles[index].acceleration = acceleration;
			break;
		}
	}
}
tsAcceleration ObjectCollection::getAcceleration(tsObjectID id)
{
	int index = getIndex(id);

	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			return texts[index].acceleration;
			break;
		case OTypeRectangle:
			return rectangles[index].acceleration;
			break;
		case OTypeCircle:
			return circles[index].acceleration;
			break;
		case OTypeTriangle:
			return triangles[index].acceleration;
			break;
		}
	}
}

void ObjectCollection::motionStep(tsObjectID id)
{
	int i = getIndex(id);

	if (i != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[i].position = { texts[i].position.x + texts[i].velocity.x + 0.5 * texts[i].acceleration.x, texts[i].position.y + texts[i].velocity.y + 0.5 * texts[i].acceleration.y };
			texts[i].velocity = { texts[i].velocity.x + texts[i].acceleration.x, texts[i].velocity.y + texts[i].acceleration.y };
			break;
		case OTypeRectangle:
			rectangles[i].position = { rectangles[i].position.x + rectangles[i].velocity.x + 0.5 * rectangles[i].acceleration.x, rectangles[i].position.y + rectangles[i].velocity.y + 0.5 * rectangles[i].acceleration.y };
			rectangles[i].velocity = { rectangles[i].velocity.x + rectangles[i].acceleration.x, rectangles[i].velocity.y + rectangles[i].acceleration.y };
			break;
		case OTypeCircle:
			circles[i].position = { circles[i].position.x + circles[i].velocity.x + 0.5 * circles[i].acceleration.x, circles[i].position.y + circles[i].velocity.y + 0.5 * circles[i].acceleration.y };
			circles[i].velocity = { circles[i].velocity.x + circles[i].acceleration.x, circles[i].velocity.y + circles[i].acceleration.y };
			break;
		case OTypeTriangle:
			triangles[i].position = { triangles[i].position.x + triangles[i].velocity.x + 0.5 * triangles[i].acceleration.x, triangles[i].position.y + triangles[i].velocity.y + 0.5 * triangles[i].acceleration.y };
			triangles[i].velocity = { triangles[i].velocity.x + triangles[i].acceleration.x, triangles[i].velocity.y + triangles[i].acceleration.y };
			break;
		}
	}
}

void ObjectCollection::motionStep()
{
	char i;

	for (i = 0; i < numberOfElementsText; i++)
	{
		texts[i].position = { texts[i].position.x + texts[i].velocity.x + 0.5 * texts[i].acceleration.x, texts[i].position.y + texts[i].velocity.y + 0.5 * texts[i].acceleration.y };
		texts[i].velocity = { texts[i].velocity.x + texts[i].acceleration.x, texts[i].velocity.y + texts[i].acceleration.y };
	}

	for (i = 0; i < numberOfElementRectangle; i++)
	{
		rectangles[i].position = { rectangles[i].position.x + rectangles[i].velocity.x + 0.5 * rectangles[i].acceleration.x, rectangles[i].position.y + rectangles[i].velocity.y + 0.5 * rectangles[i].acceleration.y };
		rectangles[i].velocity = { rectangles[i].velocity.x + rectangles[i].acceleration.x, rectangles[i].velocity.y + rectangles[i].acceleration.y };
	}

	for (i = 0; i < numberOfElementsCircle; i++)
	{
		circles[i].position = { circles[i].position.x + circles[i].velocity.x + 0.5 * circles[i].acceleration.x, circles[i].position.y + circles[i].velocity.y + 0.5 * circles[i].acceleration.y };
		circles[i].velocity = { circles[i].velocity.x + circles[i].acceleration.x, circles[i].velocity.y + circles[i].acceleration.y };
	}

	for (i = 0; i < numberOfElementsTriangle; i++)
	{
		triangles[i].position = { triangles[i].position.x + triangles[i].velocity.x + 0.5 * triangles[i].acceleration.x, triangles[i].position.y + triangles[i].velocity.y + 0.5 * triangles[i].acceleration.y };
		triangles[i].velocity = { triangles[i].velocity.x + triangles[i].acceleration.x, triangles[i].velocity.y + triangles[i].acceleration.y };
	}
}

void ObjectCollection::setColor(tsObjectID id, tsRGB color)
{
	int index = getIndex(id);
	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].color = color;
			break;
		case OTypeRectangle:
			rectangles[index].color = color;
			break;
		case OTypeCircle:
			circles[index].color = color;
			break;
		case OTypeTriangle:
			triangles[index].color = color;
			break;
		}
	}
}

void ObjectCollection::deactivate(tsObjectID id)
{
	int index = getIndex(id);
	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].isActive = false;
			break;
		case OTypeRectangle:
			rectangles[index].isActive = false;
			break;
		case OTypeCircle:
			circles[index].isActive = false;
			break;
		case OTypeTriangle:
			triangles[index].isActive = false;
			break;
		}
	}
}

void ObjectCollection::activate(tsObjectID id)
{
	int index = getIndex(id);
	if (index != -1)
	{
		switch (id.type)
		{
		case OTypeText:
			texts[index].isActive = true;
			break;
		case OTypeRectangle:
			rectangles[index].isActive = true;
			break;
		case OTypeCircle:
			circles[index].isActive = true;
			break;
		case OTypeTriangle:
			triangles[index].isActive = true;
			break;
		}
	}
}

void ObjectCollection::resetObjectCollection()
{
	if (numberOfElementsText > 0)
	{
		free(texts);
		numberOfElementsText = 0;
	}
	if (numberOfElementRectangle > 0)
	{
		free(rectangles);
		numberOfElementRectangle = 0;
	}
	if (numberOfElementsCircle > 0)
	{
		free(circles);
		numberOfElementsCircle = 0;
	}
	if (numberOfElementsTriangle > 0)
	{
		free(triangles);
		numberOfElementsTriangle = 0;
	}



}

bool ObjectCollection::checkCollision(Rectangle rectangle1, Rectangle rectangle2)
{
	return ((fabs(rectangle1.position.x - rectangle2.position.x) <= (rectangle1.length + rectangle2.length) / 2) && (fabs(rectangle1.position.y - rectangle2.position.y) <= (rectangle1.height + rectangle2.height) / 2));
}

bool ObjectCollection::checkCollision(Circle circle1, Circle circle2)
{
	return (sqrt((circle1.position.x - circle2.position.x) * (circle1.position.x - circle2.position.x) + (circle1.position.y - circle2.position.y) * (circle1.position.y - circle2.position.y)) <= (circle1.radius + circle2.radius));
}

bool ObjectCollection::checkCollision(Rectangle rectangle, Circle circle)
{
	float XLeft = rectangle.position.x - rectangle.length / 2, XRight = rectangle.position.x + rectangle.length / 2, YTop = rectangle.position.y - rectangle.height / 2, YBot = rectangle.position.y + rectangle.height / 2;
	float XLeftEdit = XLeft - circle.position.x, XRightEdit = XRight - circle.position.x, YTOPEdit = YTop - circle.position.y, YBotEdit = YBot - circle.position.y;
	float XLeftEditSquare = XLeftEdit * XLeftEdit, XRightEditSquare = XRightEdit * XRightEdit, YTOPEditSquare = YTOPEdit * YTOPEdit, YBotEditSquare = YBotEdit * YBotEdit;

	bool CircleIsVerticalyInbetween = (circle.position.y >= YTop && circle.position.y <= YBot), CircleIsHorizontalyInbetween = (circle.position.x >= XLeft && circle.position.x <= XRight);

	return (CircleIsHorizontalyInbetween <= XRight && CircleIsVerticalyInbetween) ||
		(sqrt(XLeftEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YTOPEditSquare) <= circle.radius) ||
		(sqrt(XLeftEditSquare + YBotEditSquare) <= circle.radius) ||
		(sqrt(XRightEditSquare + YBotEditSquare) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.position.y >= circle.position.y && fabs(YTOPEdit) <= circle.radius) ||
		(CircleIsHorizontalyInbetween && rectangle.position.y <= circle.position.y && fabs(YBotEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.position.x >= circle.position.x && fabs(XLeftEdit) <= circle.radius) ||
		(CircleIsVerticalyInbetween && rectangle.position.x <= circle.position.x && fabs(XRightEdit) <= circle.radius);
}

bool ObjectCollection::checkCollision(tsObjectID object1, tsObjectID object2)
{
	int index1 = getIndex(object1), index2 = getIndex(object2);


	switch (object1.type)
	{
	case OTypeRectangle:
		switch (object2.type)
		{
		case OTypeText:
			return checkCollision(rectangles[index1], Rectangle({ texts[index2].position.x + 4 * texts[index2].stringLength / 2,texts[index2].position.y }, 4, 4 * texts[index2].stringLength, { 0,0,0 }));
			break;
		case OTypeRectangle:
			return checkCollision(rectangles[index1], rectangles[index2]);
			break;
		case OTypeCircle:
			return checkCollision(rectangles[index1], circles[index2]);
			break;
		default:
			break;
		}
		break;
	case OTypeCircle:
		switch (object2.type)
		{
		case OTypeCircle:
			return checkCollision(circles[index1], circles[index2]);
			break;
		case OTypeRectangle:
			return checkCollision(rectangles[index2], circles[index1]);
			break;
		default:
			break;
		}

	default:
		break;
	}

	return false;
}

bool ObjectCollection::isInsideBorders(DPU* display, tsPosition coord)
{
	return (coord.x >= 0 && coord.x < display->resolution.x&& coord.y >= 0 && coord.y < display->resolution.y);
}

void ObjectCollection::drawObject(DPU* display, Text text)
{
	int i = 0, x_rel, y_rel, x, y;

	if (text.isActive)
	{
		while (text.string[i] != '\0' && i < 50)
		{
			for (y_rel = 0; y_rel < 4; y_rel++)
			{
				for (x_rel = 0; x_rel < 3; x_rel++)
				{
					y = round(text.position.y + y_rel);
					x = round(text.position.x + x_rel + i * 4);

					if (isInsideBorders(display, { x,y }))
					{
						if (Text4x3LUT[text.string[i] - '0'][y_rel][x_rel])
						{
							display->matrix[y][x] = text.color;
						}
					}
				}
			}
			i++;
		}
	}
}

void ObjectCollection::drawObject(DPU* display, Rectangle rectangle)
{
	int x, y;

	if (rectangle.isActive)
	{
		for (y = round(rectangle.position.y - rectangle.height / 2); y <= round(rectangle.position.y + rectangle.height / 2) - 0.5; y++)
		{
			for (x = round(rectangle.position.x - rectangle.length / 2); x <= round(rectangle.position.x + rectangle.length / 2) - 0.5; x++)
			{
				if (isInsideBorders(display, { x,y }))
				{
					display->matrix[y][x] = rectangle.color;
				}
			}
		}
	}
}

void ObjectCollection::drawObject(DPU* display, Circle circle)
{
	int x, y;

	if (circle.isActive)
	{
		for (y = round(circle.position.y - circle.radius); y <= round(circle.position.y + circle.radius); y++)
		{
			for (x = round(circle.position.x - circle.radius); x <= round(circle.position.x + circle.radius); x++)
			{
				if (isInsideBorders(display, { x,y }))
				{
					if (sqrt((circle.position.x - x) * (circle.position.x - x) + (circle.position.y - y) * (circle.position.y - y)) <= circle.radius)
					{
						display->matrix[y][x] = circle.color;
					}
				}
			}
		}
	}
}

void ObjectCollection::drawObject(DPU* display, Triangle triangle)
{
	int x, y;


}

void ObjectCollection::drawObjects(DPU* display)
{
	char i;

	for (i = 0; i < numberOfElementsText; i++)
	{
		drawObject(display, texts[i]);
	}

	for (i = 0; i < numberOfElementRectangle; i++)
	{
		drawObject(display, rectangles[i]);
	}

	for (i = 0; i < numberOfElementsCircle; i++)
	{
		drawObject(display, circles[i]);
	}

	for (i = 0; i < numberOfElementsTriangle; i++)
	{
		drawObject(display, triangles[i]);
	}
}
