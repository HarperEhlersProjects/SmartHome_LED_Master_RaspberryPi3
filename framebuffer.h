/*---------------------Module Description--------------------------
The graphics module is designated for handling all graphical calculations like color format definition and conversion, managing graphical data and color/brightness correction. 
It declares and initialize multiple arrays with RGB data as a buffer for graphical calculations.

vGprahicsInit() has to be executed once for a usage of this module.
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma once

#include "makros.h"
#include "color.h"

#include <cstdint>

typedef struct {
	float red;
	float green;
	float blue;
}tsGammaValues;

typedef struct {
	uint8_t red[GRAPHICS_GAMMA8_MAX_IN];
	uint8_t green[GRAPHICS_GAMMA8_MAX_IN];
	uint8_t blue[GRAPHICS_GAMMA8_MAX_IN];
}tsGamma8LUT;

class FrameBuffer
{
public:
	
	tsRGB buffer[GRAPHICS_DATA_SIZE];
	tsGammaValues gamma8Value;	//gamma Values for every color
	tsGamma8LUT gamma8LUT;
	bool gammaEnable;

	FrameBuffer()
	{
		long uiCount;

		for (uiCount = 0; uiCount < GRAPHICS_DATA_SIZE; uiCount++)
		{
			buffer[uiCount].uiRed = 0;
			buffer[uiCount].uiGreen = 0;
			buffer[uiCount].uiBlue = 0;
		}

		gamma8Value = { SETTINGS_GAMMA8_RED_DEFAULT ,SETTINGS_GAMMA8_GREEN_DEFAULT ,SETTINGS_GAMMA8_BLUE_DEFAULT };
		vGenerateGamma8LUT();
		gammaEnable = true;
	}

	//gamma correct one pixel with a given precalculated gamma correction LUT.
	tsRGB tsGamma8Correction(tsRGB tsColor);
	void applyGammaCorrection(void);

	//Set the RGB values of pecified pixel and SLA.
	void vSetPixel(long uiLED, tsRGB tsColorRGB);
	void vSetPixel(long uiLED, tsHSV tsColorHSV);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsRGB tsColorRGB);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsHSV tsColorHSV);
	void vResetAllPixel(void);

	//generate LUT with a given gamma value.
	void vGenerateGamma8LUT();

};

#endif /* GRAPHICS_H */
