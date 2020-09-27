#include "game_objects.h"

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


using namespace game_objects;

ObjectType GameObject::getType(void)
{
	return this->type;
}

void GameObject::setPosition(tsPosition position)
{
	this->position = position;
}
tsPosition GameObject::getPosition(void)
{
	return this->position;
}

void GameObject::setVelocity(tsVelocity velocity)
{
	this->velocity = velocity;
}
tsVelocity GameObject::getVelocity(void)
{
	return this->velocity;
}

void GameObject::setAcceleration(tsAcceleration acceleration)
{
	this->acceleration = acceleration;
}
tsAcceleration GameObject::getAcceleration(void)
{
	return this->acceleration;
}

void GameObject::setColor(tsRGB color)
{
	this->color = color;
}
tsRGB GameObject::getColor(void)
{
	return this->color;
}

void GameObject::activate(void)
{
	this->is_active = true;
}

void GameObject::deactivate(void)
{
	this->is_active = false;
}

bool GameObject::isActive(void)
{
	return this->is_active;
}

//Calculate kinetics of the object for one timestep.
void GameObject::move()
{
	this->position = { this->position.x + this->velocity.x + 0.5 * this->acceleration.x, this->position.y + this->velocity.y + 0.5 * this->acceleration.y };
	this->velocity = { this->velocity.x + this->acceleration.x, this->velocity.y + this->acceleration.y };
}

//Draw a text into a specified display matrix.
void Text::draw(DPU* display)
{
	int i = 0, x_rel, y_rel, x, y;

	if (this->isActive())
	{
		while (this->string[i] != '\0' && i < 50)
		{
			for (y_rel = 0; y_rel < 4; y_rel++)
			{
				for (x_rel = 0; x_rel < 3; x_rel++)
				{
					y = round(this->getPosition().y + y_rel);
					x = round(this->getPosition().x + x_rel + i * 4);

					if (display->isInsideBorders({ x,y }))
					{
						if (Text4x3LUT[this->string[i] - '0'][y_rel][x_rel])
						{
							display->matrix[y][x] = this->getColor();
						}
					}
				}
			}
			i++;
		}
	}
}

//Draw a rectangle into a specified display matrix.
void Rectangle::draw(DPU* display)
{
	int x, y;

	if (this->isActive())
	{
		for (y = round(this->getPosition().y - this->height / 2); y <= round(this->getPosition().y + this->height / 2) - 0.5; y++)
		{
			for (x = round(this->getPosition().x - this->length / 2); x <= round(this->getPosition().x + this->length / 2) - 0.5; x++)
			{
				if (display->isInsideBorders({ x,y }))
				{
					display->matrix[y][x] = this->getColor();
				}
			}
		}
	}
}

//Draw a circle into a specified display matrix.
void Circle::draw(DPU* display)
{
	int x, y;

	if (this->isActive())
	{
		for (y = round(this->getPosition().y - this->radius); y <= round(this->getPosition().y + this->radius); y++)
		{
			for (x = round(this->getPosition().x - this->radius); x <= round(this->getPosition().x + this->radius); x++)
			{
				if (display->isInsideBorders({ x,y }))
				{
					if (sqrt((this->getPosition().x - x) * (this->getPosition().x - x) + (this->getPosition().y - y) * (this->getPosition().y - y)) <= this->radius)
					{
						display->matrix[y][x] = this->getColor();
					}
				}
			}
		}
	}
}

void Triangle::draw(DPU* display)
{

}