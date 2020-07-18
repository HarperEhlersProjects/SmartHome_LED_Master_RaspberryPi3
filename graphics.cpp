#include "graphics.h"

#include "makros.h"
#include <math.h>


/*outdated LUT
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
*/


void GraphicsTB::vSetPixel(long uiLED,tsRGB tsColorRGB)    
{
    frameBuffer[uiLED] = tsColorRGB;
}

void GraphicsTB::vSetPixelFromTo(long uiLEDmin,long uiLEDmax,tsRGB tsColorRGB)
{
    long i;
    for(i=uiLEDmin;i<=uiLEDmax;i++)
    {
        vSetPixel(i, tsColorRGB);
    }
}

tsRGB GraphicsTB::tsGamma8Correction(tsRGB tsColorRGB)
{
	tsRGB tsTempColorRGB;
	double correction;

	tsTempColorRGB.uiRed = gamma8LUT.red[tsColorRGB.uiRed];
	tsTempColorRGB.uiGreen = gamma8LUT.green[tsColorRGB.uiGreen];
	tsTempColorRGB.uiBlue = gamma8LUT.blue[tsColorRGB.uiBlue];
	
	if (GRAPHICS_BRIGHTNESS_CORRECTION_ENABLE)
	{
		correction = 255.0 / (tsTempColorRGB.uiRed + tsTempColorRGB.uiGreen + tsTempColorRGB.uiBlue);
		tsTempColorRGB.uiRed = (char)tsTempColorRGB.uiRed * correction;
		tsTempColorRGB.uiGreen = (char)tsTempColorRGB.uiGreen * correction;
		tsTempColorRGB.uiBlue = (char)tsTempColorRGB.uiBlue * correction;
	}

	return tsTempColorRGB;
}

void GraphicsTB::vResetAllPixel(void)
{
    tsRGB tsTempColorRGB; 
	long uiSLA;
	
    tsTempColorRGB.uiRed = 0;
    tsTempColorRGB.uiGreen = 0;
    tsTempColorRGB.uiBlue = 0;
	
	vSetPixelFromTo(0,VIRTUAL_SLA_LENGTH_MAX-1,tsTempColorRGB);
}

//using formular from wikipedia "HSV-Farbraum"
tsRGB GraphicsTB::tsHSV2RGB(tsHSV tsColorHSV)
{
	tsRGB tsColorTempRGB;
	
	char h;
	double f,p,q,t,correction; 
	
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

void GraphicsTB::vGenerateGamma8LUT()
{
	long uiCount;
	char uiMax_in,uiMax_out;
	
	uiMax_in = GRAPHICS_GAMMA8_MAX_IN;
	uiMax_out = GRAPHICS_GAMMA8_MAX_OUT;
	
	for(uiCount=0;uiCount<uiMax_in;uiCount++)
	{
		gamma8LUT.red[uiCount] = (int)(pow((float) uiCount / (float) uiMax_in, gamma8Value.red) * uiMax_out + 0.5);
		gamma8LUT.green[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.green) * uiMax_out + 0.5);
		gamma8LUT.blue[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.blue) * uiMax_out + 0.5);
	}
}
