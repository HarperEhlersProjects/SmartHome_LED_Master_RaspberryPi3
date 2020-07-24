/*---------------------Module Description--------------------------
The graphics module is designated for handling all graphical calculations like color format definition and conversion, managing graphical data and color/brightness correction. 
It declares and initialize multiple arrays with RGB data as a buffer for graphical calculations.

vGprahicsInit() has to be executed once for a usage of this module.
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "makros.h"

typedef struct {
	float red;
	float green;
	float blue;
}tsGammaValues;

typedef struct {
	char red[GRAPHICS_GAMMA8_MAX_IN];
	char green[GRAPHICS_GAMMA8_MAX_IN];
	char blue[GRAPHICS_GAMMA8_MAX_IN];
}tsGamma8LUT;

//Structure that defines pixelcolor in RGB format.
typedef struct {
	char uiRed;
	char uiGreen;
	char uiBlue;
} tsRGB;

//Structure that defines pixelcolor in HSV format.
typedef struct {
	long uiHuel;
	double udSaturation;
	double udBrightness;
} tsHSV;

typedef struct {
	char x;
	char y;
} tsResolution;

typedef struct {
	float x;
	float y;
} tsPosition;

class DPU
{
public:
	tsRGB** matrix;
	tsResolution resolution;

	DPU()
	{
		this->resolution = {16,16};
		allocateMatrix();
	}

	DPU(tsResolution resolution)
	{
		this->resolution = resolution;
		allocateMatrix();
	}

	void allocateMatrix(void);

};

class GraphicsTB
{
public:
	
	tsRGB frameBuffer[GRAPHICS_DATA_SIZE];
	tsGammaValues gamma8Value;	//gamma Values for every color
	tsGamma8LUT gamma8LUT;
	bool gammaEnable;

	GraphicsTB()
	{
		long uiCount;

		for (uiCount = 0; uiCount < GRAPHICS_DATA_SIZE; uiCount++)
		{
			frameBuffer[uiCount].uiRed = 0;
			frameBuffer[uiCount].uiGreen = 0;
			frameBuffer[uiCount].uiBlue = 0;
		}

		gamma8Value = { SETTINGS_GAMMA8_RED_DEFAULT ,SETTINGS_GAMMA8_GREEN_DEFAULT ,SETTINGS_GAMMA8_BLUE_DEFAULT };
		vGenerateGamma8LUT();
		gammaEnable = true;
	}

	//gamma correct one pixel with a given precalculated gamma correction LUT.
	tsRGB tsGamma8Correction(tsRGB tsColor);
	void applyGammaCorrection(void);

	//convert color from HSV to RGB format.
	static tsRGB tsHSV2RGB(tsHSV);

	//Set the RGB values of pecified pixel and SLA.
	void vSetPixel(long uiLED, tsRGB tsColorRGB);
	void vSetPixel(long uiLED, tsHSV tsColorHSV);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsRGB tsColorRGB);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsHSV tsColorHSV);
	void vResetAllPixel(void);

	//generate LUT with a given gamma value.
	void vGenerateGamma8LUT();

};

namespace gameobjects
{
	enum ObjectType
	{
		OTypeRectangle,
		OTypeCircle,
		OTypeTriangle
	};

	class Rectangle
	{
	public:
		tsPosition position;
		float height;
		float length;

		tsRGB color;

		bool isActive;

		char idnumber;

		Rectangle(tsPosition position,char height,char length, tsRGB color)
		{
			this->position = position;
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
		float radius;

		tsRGB color;

		bool isActive;

		char idnumber;

		Circle(tsPosition position, char radius, tsRGB color)
		{
			this->position = position;
			this->radius = radius;

			this->isActive = true;
		}
	};


	class Triangle
	{
	public:
		tsPosition position;
		float baseLength;

		tsRGB color;

		bool isActive;

		int idnumber;

		Triangle(tsPosition position, char baseLength, tsRGB color)
		{
			this->position = position;
			this->baseLength = baseLength;

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
		Rectangle* rectangles;
		Circle* circles;
		Triangle* triangles;

		ObjectCollection()
		{
		}

		tsObjectID addObject(Rectangle rectangle);
		tsObjectID addObject(Circle circle);
		tsObjectID addObject(Triangle triangle);

		void setPosition(tsObjectID id,tsPosition position);
		void setColor(tsObjectID id,tsRGB color);
		void deactivate(tsObjectID);
		void activate(tsObjectID);

		void removeObject(tsObjectID id);
		int getIndex(tsObjectID id);
		void resetObjectCollection();
		
		void drawObjects(DPU* display);

		void drawObject(DPU* display, Rectangle rectangle);
		void drawObject(DPU* display, Circle circle);
		void drawObject(DPU* display, Triangle triangle);

		bool isInsideBorders(DPU* display, tsPosition coord);
	};
}

#endif /* GRAPHICS_H */
