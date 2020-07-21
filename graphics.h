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
#endif /* GRAPHICS_H */
