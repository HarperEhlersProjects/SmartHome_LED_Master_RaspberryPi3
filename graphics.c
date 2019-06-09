#include "graphics.h"

#include "makros.h"

#include "settings.h"
#include "led_serial.h"


void vGraphicsInit(void)
{
	char i,j;
	long uiCount
	
	for(uiCount=0;uiCount < GRAPHICS_DATA_SIZE;uiCount++)
	{
		puiGraphicsData[uiCount] = 0;
	}
}


void vGraphicsSetPixel(char uiSLAMask,char uiLED,char* puiColor)    
{
    char i;
    for(i=0;i<8;i++)
    {
        if((puiColor[0] & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + i] &= ~uiSLAMask;        
        }

        if((puiColor[1] & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + 8 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
        }

        if((puiColor[2] & (0x80 >> i)) > 0)
        {
            puiGraphicsData[uiLED * 24 + 16 + i] |= uiSLAMask;
        }
        else
        {
            puiGraphicsData[uiLED * 24 + 16 + i] &= ~uiSLAMask;
        }
    }
}

void vGraphicsSetPixelFromTo(char uiSLAMask,char uiLEDmin,char uiLEDmax,char* puiColor)
{
    char i;
    for(i=uiLEDmin;i<=uiLEDmax;i++)
    {
        vGraphicsSetPixel(uiSLAMask, i, puiColor);
    }
}

void vGraphicsPixelResetAll(void)
{
    char puiColor[3];
    puiColor[0] = 0;
    puiColor[1] = 0;
    puiColor[2] = 0;

    vGraphicsSetPixelFromTo(0xFF,0,SLA_LENGTH_MAX-1,puiColor);
}


