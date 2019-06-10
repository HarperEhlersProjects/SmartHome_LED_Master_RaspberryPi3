#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "makros.h"

char puiGraphicsData[GRAPHICS_DATA_SIZE];

void vGraphicsInit(void);

void vGraphicsGamma8Correction(char* puiColor);

void vGraphicsSetPixel(char uiSLAMask,long uiLED,char* puiColor);
void vGraphicsSetPixelFromTo(char uiSLAMask,long uiLEDmin,long uiLEDmax,char* puiColor);
void vGraphicsPixelResetAll(void);


#endif /* GRAPHICS_H */
