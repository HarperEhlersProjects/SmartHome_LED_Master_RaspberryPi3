#ifndef GRAPHICS_H
#define GRAPHICS_H

void vGraphicsInit(void);
void vGraphicsFrameCalculate(void);
void vGraphicsSetPixel(char uiSLAMask,char uiLED,char* puiColor);
void vGraphicsSetPixelFromTo(char uiSLAMask,char uiLEDmin,char uiLEDmax,char* puiColor);
void vGraphicsPixelResetAll(void);

void vGraphicsMode0(char uiSLA);
void vGraphicsMode1(char uiSLA);
void vGraphicsMode2(char uiSLA);
void vGraphicsMode3(char uiSLA);
#endif /* GRAPHICS_H */
