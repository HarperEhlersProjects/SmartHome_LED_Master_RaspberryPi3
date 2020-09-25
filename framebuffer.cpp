#include "framebuffer.h"

#include "color.h"

#include "makros.h"
#include <math.h>
#include <stdlib.h>
#include <cstdint>

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


//Set pixel in the framebuffer with a specified color in rgb format
void FrameBuffer::vSetPixel(long uiLED,tsRGB tsColorRGB)    
{
	if (gammaEnable)
	{
		buffer[uiLED] = tsGamma8Correction(tsColorRGB);
	}
	else
	{
		buffer[uiLED] = tsColorRGB;
	}
}

//Set pixel in the framebuffer with a specified color in hsv format
void FrameBuffer::vSetPixel(long uiLED, tsHSV tsColorHSV)
{
	vSetPixel(uiLED, tsHSV2RGB(tsColorHSV));

}

//Set all pixels in the framebuffer between two indices with a specified color in rgb format
void FrameBuffer::vSetPixelFromTo(long uiLEDmin,long uiLEDmax,tsRGB tsColorRGB)
{
    for(uint16_t i=uiLEDmin;i<=uiLEDmax;i++)
    {
        vSetPixel(i, tsColorRGB);
    }
}

//Set all pixels in the framebuffer between two indices with a specified color in hsv format
void FrameBuffer::vSetPixelFromTo(long uiLEDmin, long uiLEDmax, tsHSV tsColorHSV)
{
	vSetPixelFromTo(uiLEDmin, uiLEDmax, tsHSV2RGB(tsColorHSV));
}

//Apply gamma correction to a pixel. This method is using pregenerated gamma LUT
tsRGB FrameBuffer::tsGamma8Correction(tsRGB tsColorRGB)
{
	tsRGB tsTempColorRGB;
	double correction;

	tsTempColorRGB.uiRed = gamma8LUT.red[tsColorRGB.uiRed];
	tsTempColorRGB.uiGreen = gamma8LUT.green[tsColorRGB.uiGreen];
	tsTempColorRGB.uiBlue = gamma8LUT.blue[tsColorRGB.uiBlue];
	
	//CODE IN TESTING PHASE
	if (GRAPHICS_BRIGHTNESS_CORRECTION_ENABLE)
	{
		correction = 255.0 / (tsTempColorRGB.uiRed + tsTempColorRGB.uiGreen + tsTempColorRGB.uiBlue);
		tsTempColorRGB.uiRed = (char)tsTempColorRGB.uiRed * correction;
		tsTempColorRGB.uiGreen = (char)tsTempColorRGB.uiGreen * correction;
		tsTempColorRGB.uiBlue = (char)tsTempColorRGB.uiBlue * correction;
	}

	return tsTempColorRGB;
}

//Apply gamma correction to the whole framebuffer
void FrameBuffer::applyGammaCorrection()
{
	for (uint16_t i = 0; i < GRAPHICS_DATA_SIZE; i++)
	{
		buffer[i] = tsGamma8Correction(buffer[i]);
	}
}

//Reset framebuffer
void FrameBuffer::vResetAllPixel(void)
{
	vSetPixelFromTo(0, VIRTUAL_SLA_LENGTH_MAX - 1, tsRGB {0,0,0});
}


//Initialize 8 bit gamma LUT
void FrameBuffer::vGenerateGamma8LUT()
{
	char uiMax_in,uiMax_out;
	
	uiMax_in = GRAPHICS_GAMMA8_MAX_IN;
	uiMax_out = GRAPHICS_GAMMA8_MAX_OUT;
	
	for(uint16_t uiCount=0 ; uiCount < uiMax_in ; uiCount++)
	{
		gamma8LUT.red[uiCount] = (int)(pow((float) uiCount / (float) uiMax_in, gamma8Value.red) * uiMax_out + 0.5);
		gamma8LUT.green[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.green) * uiMax_out + 0.5);
		gamma8LUT.blue[uiCount] = (int)(pow((float)uiCount / (float)uiMax_in, gamma8Value.blue) * uiMax_out + 0.5);
	}
}