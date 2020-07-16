/*---------------------Module Description--------------------------
The graphics module is designated for handling all graphical calculations like color format definition and conversion, managing graphical data and color/brightness correction. 
It declares and initialize multiple arrays with RGB data as a buffer for graphical calculations.

vGprahicsInit() has to be executed once for a usage of this module.
*/



#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "makros.h"

//Structure that defines pixelcolor in RGB format.
typedef struct{	
char uiRed;
char uiGreen;
char uiBlue;
} tsGraphicsRGB;

//Structure that defines pixelcolor in HSV format.
typedef struct{
long uiHuel;
double udSaturation;
double udBrightness;
} tsGraphicsHSV;

//Pixeldata array in RGB format for every SLA.
extern tsGraphicsRGB puiGraphicsData[VIRTUAL_SLA_NUMBER][GRAPHICS_DATA_SIZE];

//Initialize graphics module.
void vGraphicsInit(void);

//gamma correct one pixel with a given precalculated gamma correction LUT.
tsGraphicsRGB tsGraphicsGamma8Correction(char uiSLA, tsGraphicsRGB tsColor);

//Set the RGB values of pecified pixel and SLA.
void vGraphicsSetPixel(char uiSLA,long uiLED,tsGraphicsRGB tsColorRGB);
void vGraphicsSetPixelFromTo(char uiSLA,long uiLEDmin,long uiLEDmax,tsGraphicsRGB tsColorRGB);
void vGraphicsPixelResetAll(void);

//generate LUT with a given gamma value.
void vGraphicsGenerateGamma8LUT(float fGamma, char* puiLUT);

//convert color from HSV to RGB format.
tsGraphicsRGB tsGraphicsHSV2RGB(tsGraphicsHSV);
#endif /* GRAPHICS_H */
