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
unsigned double udSaturation;
unsigned double udBrightness;
} tsGraphicsHSV;

char puiGraphicsData[GRAPHICS_DATA_SIZE];

void vGraphicsInit(void);

tsGraphicsRGB tsGraphicsGamma8Correction(tsGraphicsRGB tsColor);

void vGraphicsSetPixel(char uiSLAMask,long uiLED,tsGraphicsRGB tsColorRGB);
void vGraphicsSetPixelFromTo(char uiSLAMask,long uiLEDmin,long uiLEDmax,tsGraphicsRGB tsColorRGB);
void vGraphicsPixelResetAll(void);

tsGraphicsRGB tsGraphicsHSV2RGB(tsGraphicsHSV);
#endif /* GRAPHICS_H */
