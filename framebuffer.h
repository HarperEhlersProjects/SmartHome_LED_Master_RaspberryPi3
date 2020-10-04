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
#include <vector>

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
	//tsRGB buffer[VIRTUAL_SLA_LENGTH_MAX] = {0,0,0};
	
	tsGammaValues gamma8Value;	//gamma Values for every color
	tsGamma8LUT gamma8LUT;
	bool gammaEnable;

	FrameBuffer();
	FrameBuffer(long size);

	void setSize(long size);
	long size(void);

	//gamma correct one pixel with a given precalculated gamma correction LUT.
	tsRGB tsGamma8Correction(tsRGB tsColor);
	void applyGammaCorrection(void);

	//Set the RGB values of pecified pixel and SLA.
	void vSetPixel(long uiLED, tsRGB tsColorRGB);
	void vSetPixel(long uiLED, tsHSV tsColorHSV);
	tsRGB getPixel(long uiLED);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsRGB tsColorRGB);
	void vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsHSV tsColorHSV);
	void vResetAllPixel(void);

	//generate LUT with a given gamma value.
	void vGenerateGamma8LUT();

private:
	std::vector<tsRGB> buffer;
};

#endif /* GRAPHICS_H */
