#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "makros.h"

typedef struct{	
char uiRed;
char uiGreen;
char uiBlue;
} tsGraphicsRGB;

typedef struct{
long uiHuel;
double udSaturation;
double udBrightness;
} tsGraphicsHSV;

tsGraphicsRGB puiGraphicsData[SLA_NUMBER][GRAPHICS_DATA_SIZE];

void vGraphicsInit(void);

tsGraphicsRGB tsGraphicsGamma8Correction(char uiSLA, tsGraphicsRGB tsColor);

void vGraphicsSetPixel(char uiSLA,long uiLED,tsGraphicsRGB tsColorRGB);
void vGraphicsSetPixelFromTo(char uiSLA,long uiLEDmin,long uiLEDmax,tsGraphicsRGB tsColorRGB);
void vGraphicsPixelResetAll(void);

void vGraphicsGenerateGamma8LUT(float fGamma, char* puiLUT);

tsGraphicsRGB tsGraphicsHSV2RGB(tsGraphicsHSV);
#endif /* GRAPHICS_H */
