#include "graphics.h"

#include "makros.h"
#include <math.h>
#include <stdlib.h>

/*outdated LUT
const char puiGraphicsGamma8CorrectionLUT[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
*/
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


const bool Text4x3LUT[43][4][3] = { TEXT_0,TEXT_1,TEXT_2,TEXT_3,TEXT_4,TEXT_5,TEXT_6,TEXT_7,TEXT_8,TEXT_9,
									TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,TEXT_BLANK,
									TEXT_A,TEXT_B,TEXT_C,TEXT_D,TEXT_E,TEXT_F,TEXT_G,TEXT_H,TEXT_I,TEXT_J,
									TEXT_K,TEXT_L,TEXT_M,TEXT_N,TEXT_O,TEXT_P,TEXT_Q,TEXT_R,TEXT_S,TEXT_T,
									TEXT_U,TEXT_V,TEXT_W,TEXT_X,TEXT_Y,TEXT_Z };

void GraphicsTB::vSetPixel(long uiLED,tsRGB tsColorRGB)    
{
	if (gammaEnable)
	{
		frameBuffer[uiLED] = tsGamma8Correction(tsColorRGB);
	}
	else
	{
		frameBuffer[uiLED] = tsColorRGB;
	}
}

void GraphicsTB::vSetPixel(long uiLED, tsHSV tsColorHSV)
{
	vSetPixel(uiLED, tsHSV2RGB(tsColorHSV));

}

void GraphicsTB::vSetPixelFromTo(long uiLEDmin,long uiLEDmax,tsRGB tsColorRGB)
{
    long i;
    for(i=uiLEDmin;i<=uiLEDmax;i++)
    {
        vSetPixel(i, tsColorRGB);
    }
}

void GraphicsTB::vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsHSV tsColorHSV)
{
	vSetPixelFromTo(uiLEDmin, uiLEDmax, tsHSV2RGB(tsColorHSV));
}

tsRGB GraphicsTB::tsGamma8Correction(tsRGB tsColorRGB)
{
	tsRGB tsTempColorRGB;
	double correction;

	tsTempColorRGB.uiRed = gamma8LUT.red[tsColorRGB.uiRed];
	tsTempColorRGB.uiGreen = gamma8LUT.green[tsColorRGB.uiGreen];
	tsTempColorRGB.uiBlue = gamma8LUT.blue[tsColorRGB.uiBlue];
	
	if (GRAPHICS_BRIGHTNESS_CORRECTION_ENABLE)
	{
		correction = 255.0 / (tsTempColorRGB.uiRed + tsTempColorRGB.uiGreen + tsTempColorRGB.uiBlue);
		tsTempColorRGB.uiRed = (char)tsTempColorRGB.uiRed * correction;
		tsTempColorRGB.uiGreen = (char)tsTempColorRGB.uiGreen * correction;
		tsTempColorRGB.uiBlue = (char)tsTempColorRGB.uiBlue * correction;
	}

	return tsTempColorRGB;
}

void GraphicsTB::applyGammaCorrection()
{
	char i;

	for (i = 0; i < GRAPHICS_DATA_SIZE; i++)
	{
		frameBuffer[i] = tsGamma8Correction(frameBuffer[i]);
	}
}

void GraphicsTB::vResetAllPixel(void)
{
    tsRGB tsTempColorRGB; 
	long uiSLA;
	
    tsTempColorRGB.uiRed = 0;
    tsTempColorRGB.uiGreen = 0;
    tsTempColorRGB.uiBlue = 0;
	
	vSetPixelFromTo(0,VIRTUAL_SLA_LENGTH_MAX-1,tsTempColorRGB);
}

//using formular from wikipedia "HSV-Farbraum"
tsRGB GraphicsTB::tsHSV2RGB(tsHSV tsColorHSV)
{
	tsRGB tsColorTempRGB;
	
	char h;
	double f,p,q,t,correction; 
	
	h = (char) (tsColorHSV.uiHuel/60.0);
	f = (tsColorHSV.uiHuel/60.0-h);
	
	p = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation);
	q = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation * f);
	t = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation * (1 - f));
	
	switch(h)
	{
		case 0: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (t * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
		break;
		case 1: tsColorTempRGB.uiRed = (char) (q * 255);
				tsColorTempRGB.uiGreen = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
		break;
		case 2: tsColorTempRGB.uiRed = (char) (p * 255);
				tsColorTempRGB.uiGreen = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiBlue = (char) (t * 255);
		break;
		case 3: tsColorTempRGB.uiRed = (char) (p * 255);
				tsColorTempRGB.uiGreen = (char) (q * 255);
				tsColorTempRGB.uiBlue = (char) (tsColorHSV.udBrightness * 255);
		break;
		case 4: tsColorTempRGB.uiRed = (char) (t * 255);
				tsColorTempRGB.uiGreen = (char) (p * 255);
				tsColorTempRGB.uiBlue = (char) (tsColorHSV.udBrightness * 255);
		break;
		case 5: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (p * 255);
				tsColorTempRGB.uiBlue = (char) (q * 255);
		break;
		case 6: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (t * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
	}

	return tsColorTempRGB;
}

void GraphicsTB::vGenerateGamma8LUT()
{
	long uiCount;
	char uiMax_in,uiMax_out;
	
	uiMax_in = GRAPHICS_GAMMA8_MAX_IN;
	uiMax_out = GRAPHICS_GAMMA8_MAX_OUT;
	
	for(uiCount=0;uiCount<uiMax_in;uiCount++)
	{
		gamma8LUT.red[uiCount] = (int)(pow((float) uiCount / (float) uiMax_in, gamma8Value.red) * uiMax_out + 0.5);
		gamma8LUT.green[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.green) * uiMax_out + 0.5);
		gamma8LUT.blue[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.blue) * uiMax_out + 0.5);
	}
}

gameobjects::Text::Text(tsPosition position, char* string, tsRGB color)
{
	char i=0;

	while (string[i] != '\0' && i<50)
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


void DPU::allocateMatrix()
{
	char i;

	this->matrix = (tsRGB**) malloc(this->resolution.y * sizeof(tsRGB*));

	for (i = 0; i < this->resolution.y; i++)
	{
		matrix[i] = (tsRGB*) malloc(this->resolution.y * sizeof(tsRGB));
	}
}

void DPU::resetMatrix()
{
	char x,y;

	for (y = 0; y < this->resolution.y; y++)
	{
		for (x = 0; x < this->resolution.x; x++)
		{
			matrix[y][x] = {0, 0, 0};
		}
	}
}

int gameobjects::ObjectCollection::getIndex(tsObjectID id)
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


gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Text text)
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

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Rectangle rectangle)
{
	char newIndex = numberOfElementRectangle;
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeRectangle}) != -1)
	{
		lowestAvailableID++;
	}

	this->rectangles = (Rectangle*) realloc(this->rectangles, sizeof(Rectangle) * (newIndex + 1));
	numberOfElementRectangle++;
	rectangle.idnumber = lowestAvailableID;
	rectangles[newIndex] = rectangle;

	return {lowestAvailableID,OTypeRectangle};
}

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Circle circle)
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

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Triangle triangle)
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

void gameobjects::ObjectCollection::removeObject(tsObjectID id)
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

void gameobjects::ObjectCollection::setPosition(tsObjectID id, tsPosition position)
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

tsPosition gameobjects::ObjectCollection::getPosition(tsObjectID id)
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

void gameobjects::ObjectCollection::setVelocity(tsObjectID id, tsVelocity velocity)
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
tsVelocity gameobjects::ObjectCollection::getVelocity(tsObjectID id)
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
void gameobjects::ObjectCollection::setAcceleration(tsObjectID id, tsAcceleration acceleration)
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
tsAcceleration gameobjects::ObjectCollection::getAcceleration(tsObjectID id)
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

void gameobjects::ObjectCollection::motionStep(tsObjectID id)
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

void gameobjects::ObjectCollection::motionStep()
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

void gameobjects::ObjectCollection::setColor(tsObjectID id, tsRGB color)
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

void gameobjects::ObjectCollection::deactivate(tsObjectID id)
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

void gameobjects::ObjectCollection::activate(tsObjectID id)
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

void gameobjects::ObjectCollection::resetObjectCollection()
{
	if(numberOfElementsText>0)
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

bool gameobjects::ObjectCollection::checkCollision(Rectangle rectangle1, Rectangle rectangle2)
{
	return ((fabs(rectangle1.position.x - rectangle2.position.x) <= (rectangle1.length + rectangle2.length) / 2 ) && (fabs(rectangle1.position.y - rectangle2.position.y) <= (rectangle1.height + rectangle2.height) / 2)) ;
}

bool gameobjects::ObjectCollection::checkCollision(Circle circle1, Circle circle2)
{
	return (sqrt((circle1.position.x - circle2.position.x) * (circle1.position.x - circle2.position.x) + (circle1.position.y - circle2.position.y) * (circle1.position.y - circle2.position.y)) <= (circle1.radius + circle2.radius));
}

bool gameobjects::ObjectCollection::checkCollision(Rectangle rectangle, Circle circle)
{
	float XLeft = rectangle.position.x - rectangle.length / 2, XRight = rectangle.position.x + rectangle.length / 2, YTop = rectangle.position.y - rectangle.height / 2, YBot = rectangle.position.y + rectangle.height / 2;
	float XLeftEdit = XLeft - circle.position.x, XRightEdit = XRight - circle.position.x, YTOPEdit = YTop - circle.position.y, YBotEdit = YBot - circle.position.y;
	float XLeftEditSquare = XLeftEdit*XLeftEdit, XRightEditSquare = XRightEdit* XRightEdit, YTOPEditSquare = YTOPEdit* YTOPEdit, YBotEditSquare = YBotEdit* YBotEdit;

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

bool gameobjects::ObjectCollection::checkCollision(tsObjectID object1, tsObjectID object2)
{
	int index1 = getIndex(object1), index2 = getIndex(object2);


	switch (object1.type)
	{
	case OTypeRectangle:
		switch (object2.type)
		{
		case OTypeText:
			return checkCollision(rectangles[index1], Rectangle({ texts[index2].position.x + 4 * texts[index2].stringLength/2,texts[index2].position.y },4,4*texts[index2].stringLength,{0,0,0}));
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

bool gameobjects::ObjectCollection::isInsideBorders(DPU* display, tsPosition coord)
{
	return (coord.x >= 0 && coord.x < display->resolution.x&& coord.y >= 0 && coord.y < display->resolution.y);
}

void gameobjects::ObjectCollection::drawObject(DPU* display, Text text)
{
	int i=0, x_rel, y_rel,x,y;

	if (text.isActive)
	{
		while(text.string[i] != '\0' && i<50) 
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

void gameobjects::ObjectCollection::drawObject(DPU* display, Rectangle rectangle)
{
	int x,y;

	if(rectangle.isActive)
	{ 
		for (y=round(rectangle.position.y - rectangle.height/2); y <= round(rectangle.position.y + rectangle.height / 2) - 0.5; y++)
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

void gameobjects::ObjectCollection::drawObject(DPU* display, Circle circle)
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

void gameobjects::ObjectCollection::drawObject(DPU* display, Triangle triangle)
{
	int x, y;


}

void gameobjects::ObjectCollection::drawObjects(DPU* display)
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