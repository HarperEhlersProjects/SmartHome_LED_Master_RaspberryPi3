#ifndef GRAPHICS_H
#define GRAPHICS_H

char puiGraphicsData[Graphics_DATA_SIZE];

void vGraphicsInit(void);

void vGraphicsSetPixel(char uiSLAMask,char uiLED,char* puiColor);
void vGraphicsSetPixelFromTo(char uiSLAMask,char uiLEDmin,char uiLEDmax,char* puiColor);
void vGraphicsPixelResetAll(void);


#endif /* GRAPHICS_H */
