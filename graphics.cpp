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

void DPU::allocateMatrix()
{
	char i;

	this->matrix = (tsRGB**) malloc(this->resolution.y * sizeof(tsRGB*));

	for (i = 0; i < this->resolution.y; i++)
	{
		matrix[i] = (tsRGB*) malloc(this->resolution.y * sizeof(tsRGB));
	}
}


int gameobjects::ObjectCollection::getIndex(tsObjectID id)
{
	char i, size;

	switch (id.type)
	{
	case OTypeRectangle:

		size = sizeof(rectangles) / sizeof(Rectangle);
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

		size = sizeof(circles) / sizeof(Circle);
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

		size = sizeof(triangles) / sizeof(Triangle);
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

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Rectangle rectangle)
{
	char newIndex = sizeof(rectangles) / sizeof(Rectangle);
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeRectangle}) != -1)
	{
		lowestAvailableID++;
	}

	this->rectangles = (Rectangle*) realloc(this->rectangles, sizeof(Rectangle) * (newIndex + 1));
	rectangle.idnumber = lowestAvailableID;
	rectangles[newIndex] = rectangle;

	return {lowestAvailableID,OTypeRectangle};
}

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Circle circle)
{
	char newIndex = sizeof(rectangles) / sizeof(Circle);
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeCircle }) != -1)
	{
		lowestAvailableID++;
	}

	this->circles = (Circle*)realloc(this->circles, sizeof(Circle) * (newIndex + 1));
	circle.idnumber = lowestAvailableID;
	circles[newIndex] = circle;

	return { lowestAvailableID,OTypeCircle };
}

gameobjects::tsObjectID gameobjects::ObjectCollection::addObject(Triangle triangle)
{
	char newIndex = sizeof(triangles) / sizeof(Triangle);
	int lowestAvailableID = 0;

	while (getIndex({ lowestAvailableID , OTypeTriangle }) != -1)
	{
		lowestAvailableID++;
	}

	this->triangles = (Triangle*)realloc(this->triangles, sizeof(Triangle) * (newIndex + 1));
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
		case OTypeRectangle:
			size = sizeof(rectangles) / sizeof(Rectangle);

			rectangles[index] = rectangles[size - 1];
			rectangles = (Rectangle*)realloc(rectangles, sizeof(Rectangle) * (size - 1));
			break;
		case OTypeCircle:

			size = sizeof(circles) / sizeof(Circle);

			circles[index] = circles[size - 1];
			circles = (Circle*)realloc(circles, sizeof(Circle) * (size - 1));

			break;
		case OTypeTriangle:

			size = sizeof(triangles) / sizeof(Triangle);

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

void gameobjects::ObjectCollection::setColor(tsObjectID id, tsRGB color)
{
	int index = getIndex(id);
	if (index != -1)
	{
		switch (id.type)
		{
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
	free(rectangles);
	free(circles);
	free(triangles);
}

bool isInsideBorders(DPU* display, tsPosition coord)
{
	return (coord.x > 0 && coord.x < display->resolution.x&& coord.y > 0 && coord.y < display->resolution.y);
}

void gameobjects::ObjectCollection::drawObject(DPU* display, Rectangle rectangle)
{
	int x,y;

	if(rectangle.isActive)
	{ 
		for (y=round(rectangle.position.y - rectangle.height/2); y <= round(rectangle.position.y + rectangle.height / 2); y++)
		{
			for (x = round(rectangle.position.x - rectangle.length / 2); x <= round(rectangle.position.x + rectangle.length / 2); x++)
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

	for (i = 0; i < sizeof(rectangles) / sizeof(Rectangle); i++)
	{
		drawObject(display, rectangles[i]);
	}

	for (i = 0; i < sizeof(circles) / sizeof(Circle); i++)
	{
		drawObject(display, circles[i]);
	}

	for (i = 0; i < sizeof(triangles) / sizeof(Triangle); i++)
	{
		drawObject(display, triangles[i]);
	}
}