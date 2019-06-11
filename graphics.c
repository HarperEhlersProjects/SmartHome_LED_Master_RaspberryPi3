#include "graphics.h"

#include "makros.h"

#include "settings.h"
#include "led_serial.h"

#include <math.h>

char puiGraphicsGamma8CorrectionLUTRed[SLA_NUMBER][GRAPHICS_GAMMA8_MAX_IN];
char puiGraphicsGamma8CorrectionLUTGreen[SLA_NUMBER][GRAPHICS_GAMMA8_MAX_IN];
char puiGraphicsGamma8CorrectionLUTBlue[SLA_NUMBER][GRAPHICS_GAMMA8_MAX_IN];

const char puiGraphicsGamma8CorrectionLUT[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void vGraphicsInit(void)
{
	long uiCount;
	
	for(uiCount=0;uiCount < GRAPHICS_DATA_SIZE;uiCount++)
	{
		puiGraphicsData[uiCount] = 0;
	}
	
	//Generate gamma correction tables
	for(uiCount=0;uiCount < SLA_NUMBER;uiCount++)
	{
		vGraphicsGenerateGamma8LUT(fSettingsGamma8RedValue[uiCount], puiGraphicsGamma8CorrectionLUTRed[uiCount]);
		vGraphicsGenerateGamma8LUT(fSettingsGamma8GreenValue[uiCount], puiGraphicsGamma8CorrectionLUTGreen[uiCount]);
		vGraphicsGenerateGamma8LUT(fSettingsGamma8BlueValue[uiCount], puiGraphicsGamma8CorrectionLUTBlue[uiCount]);
	}
}


void vGraphicsSetPixel(char uiSLAMask,long uiLED,tsGraphicsRGB tsColorRGB)    
{
    long i;
    for(i=0;i<8;i++)
    {
        if((tsColorRGB.uiRed & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + i] &= ~uiSLAMask;        
        }

        if((tsColorRGB.uiGreen & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + 8 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
        }

        if((tsColorRGB.uiBlue & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + 16 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + 16 + i] &= ~uiSLAMask;
        }
    }
}

void vGraphicsSetPixelFromTo(char uiSLAMask,long uiLEDmin,long uiLEDmax,tsGraphicsRGB tsColorRGB)
{
    long i;
    for(i=uiLEDmin;i<=uiLEDmax;i++)
    {
        vGraphicsSetPixel(uiSLAMask, i, tsColorRGB);
    }
}

tsGraphicsRGB tsGraphicsGamma8Correction(char uiSLA, tsGraphicsRGB tsColorRGB)
{
	tsGraphicsRGB tsTempColorRGB;
	
	tsTempColorRGB.uiRed = puiGraphicsGamma8CorrectionLUTRed[uiSLA][tsColorRGB.uiRed];
	tsTempColorRGB.uiGreen = puiGraphicsGamma8CorrectionLUTGreen[uiSLA][tsColorRGB.uiGreen];
	tsTempColorRGB.uiBlue = puiGraphicsGamma8CorrectionLUTBlue[uiSLA][tsColorRGB.uiBlue];
	
	return tsTempColorRGB;
}

void vGraphicsPixelResetAll(void)
{
    tsGraphicsRGB tsTempColorRGB; 
	
    tsTempColorRGB.uiRed = 0;
    tsTempColorRGB.uiGreen = 0;
    tsTempColorRGB.uiBlue = 0;

    vGraphicsSetPixelFromTo(0xFF,0,SLA_LENGTH_MAX-1,tsTempColorRGB);
}

//using formular from wikipedia "HSV-Farbraum"
tsGraphicsRGB tsGraphicsHSV2RGB(tsGraphicsHSV tsColorHSV)
{
	tsGraphicsRGB tsColorTempRGB;
	
	char h;
	double f,p,q,t; 
	
	h = (char) (tsColorHSV.uiHuel/60.0);
	f = (tsColorHSV.uiHuel/60.0-h);
	
	p = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation);
	q = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation * f);
	t = tsColorHSV.udBrightness*(1 - tsColorHSV.udSaturation * (1 - f));
	
	switch(h)
	{
		case 0: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (t * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
		break;
		case 1: tsColorTempRGB.uiRed = (char) (q * 255);
				tsColorTempRGB.uiGreen = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
		break;
		case 2: tsColorTempRGB.uiRed = (char) (p * 255);
				tsColorTempRGB.uiGreen = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiBlue = (char) (t * 255);
		break;
		case 3: tsColorTempRGB.uiRed = (char) (p * 255);
				tsColorTempRGB.uiGreen = (char) (q * 255);
				tsColorTempRGB.uiBlue = (char) (tsColorHSV.udBrightness * 255);
		break;
		case 4: tsColorTempRGB.uiRed = (char) (t * 255);
				tsColorTempRGB.uiGreen = (char) (p * 255);
				tsColorTempRGB.uiBlue = (char) (tsColorHSV.udBrightness * 255);
		break;
		case 5: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (p * 255);
				tsColorTempRGB.uiBlue = (char) (q * 255);
		break;
		case 6: tsColorTempRGB.uiRed = (char) (tsColorHSV.udBrightness * 255);
				tsColorTempRGB.uiGreen = (char) (t * 255);
				tsColorTempRGB.uiBlue = (char) (p * 255);
	}
	
	return tsColorTempRGB;
}

void vGraphicsGenerateGamma8LUT(float fGamma, char* puiLUT)
{
	long uiCount;
	char uiMax_in,uiMax_out;
	
	uiMax_in = GRAPHICS_GAMMA8_MAX_IN;
	uiMax_out = GRAPHICS_GAMMA8_MAX_OUT;
	
	for(uiCount=0;uiCount<uiMax_in;uiCount++)
	{
		puiLUT[uiCount] = (int)(pow((float) uiCount / (float) uiMax_in, fGamma) * uiMax_out + 0.5);
	}
}
