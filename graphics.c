#include "graphics.h"

#include "makros.h"

#include "settings.h"
#include "led_serial.h"

int piModeActors[SLA_NUMBER][MODE_ACTOR_NUMBER];


void vGraphicsInit(void)
{
	char i,j;
	
	for(i=0;i<SLA_NUMBER;i++)
	{
		for(j=0;j<MODE_ACTOR_NUMBER;j++)
		{
			piModeActors[i][j] = 0;
		}
	}
}

void vGraphicsFrameCalculate()
{
	char uiCounterSLA;
	
	for(uiCounterSLA=0;uiCounterSLA < SLA_NUMBER;uiCounterSLA++)
	{
		switch(uiSettingsModeActive[uiCounterSLA])
        {
            case 0:vGraphicsMode0(uiCounterSLA);//LED off
            break;
            case 1:vGraphicsMode1(uiCounterSLA);//constant RGB
            break;
            case 2:vGraphicsMode2(uiCounterSLA);//moving snake RGB
            break;
            case 3:vGraphicsMode3(uiCounterSLA);//Pulse
            break;
		}
	}
}

void vGraphicsSetPixel(char uiSLAMask,char uiLED,char* puiColor)    
{
    char i;
    for(i=0;i<8;i++)
    {
        if((puiColor[0] & (0x80 >> i)) > 0)
        {
            puiLEDSerialData[uiLED * 24 + i] |= uiSLAMask;
        }
        else
        {
            puiLEDSerialData[uiLED * 24 + i] &= ~uiSLAMask;        
        }

        if((puiColor[1] & (0x80 >> i)) > 0)
        {
            puiLEDSerialData[uiLED * 24 + 8 + i] |= uiSLAMask;
        }
        else
        {
            puiLEDSerialData[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
        }

        if((puiColor[2] & (0x80 >> i)) > 0)
        {
            puiLEDSerialData[uiLED * 24 + 16 + i] |= uiSLAMask;
        }
        else
        {
            puiLEDSerialData[uiLED * 24 + 16 + i] &= ~uiSLAMask;
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

void vGraphicsMode0(char uiSLA)
{

}

void vGraphicsMode1(char uiSLA)
{

    char puiColor[3];
    long uiCounter;

	puiColor[0] = uiSettingsModeParameter[uiSLA][0];
	puiColor[1] = uiSettingsModeParameter[uiSLA][1];
	puiColor[2] = uiSettingsModeParameter[uiSLA][2];

    for(uiCounter=0;uiCounter<uiSettingsSLALength[uiSLA];uiCounter++)
    {
        vGraphicsSetPixel(1<<uiSLA,uiCounter,puiColor);
    }
}

void vGraphicsMode2(char uiSLA)
{
    char puiColor[3];
    long uiCounter;
    long uiTopBoundary,uiBottomBoundary;


	puiColor[0] = uiSettingsModeParameter[uiSLA][0];
	puiColor[1] = uiSettingsModeParameter[uiSLA][1];
	puiColor[2] = uiSettingsModeParameter[uiSLA][2];

    piModeActors[uiSLA][1]++;

    if(piModeActors[uiSLA][1] > 255-uiSettingsModeParameter[uiSLA][4])
    {
        piModeActors[uiSLA][0]++;

        if(piModeActors[uiSLA][0] >= uiSettingsSLALength[uiSLA] + uiSettingsModeParameter[uiSLA][3])
        {
            piModeActors[uiSLA][0] = 0;
        }

        piModeActors[uiSLA][1] = 0;
    }

    if(piModeActors[uiSLA][0] - uiSettingsModeParameter[uiSLA][3] < 0)  //cut if its too long for SLA
    {
        uiBottomBoundary = 0;
    }
    else
    {
        uiBottomBoundary = piModeActors[uiSLA][0] - uiSettingsModeParameter[uiSLA][3];
    }

    uiTopBoundary=piModeActors[uiSLA][0];

    if(uiTopBoundary >= uiSettingsSLALength[uiSLA])
    {
        uiTopBoundary = uiSettingsSLALength[uiSLA];

    }

    for(uiCounter = uiBottomBoundary;uiCounter < uiTopBoundary;uiCounter++)
    {
        vGraphicsSetPixel(1<<uiSLA,uiCounter,puiColor);
    }
}

void vGraphicsMode3(char uiSLA)
{
    char puiColor[3];

    puiColor[uiSettingsModeParameter[uiSLA][0]] = 0;
    puiColor[uiSettingsModeParameter[uiSLA][1]] = 0.35*(uiSettingsSLALength[uiSLA]/2-piModeActors[uiSLA][0]);
    puiColor[uiSettingsModeParameter[uiSLA][2]] = 0.35*piModeActors[uiSLA][0];

    if(piModeActors[uiSLA][0] > uiSettingsSLALength[uiSLA]/2)
    {   
        piModeActors[uiSLA][1]=0;
    }
    else if(piModeActors[uiSLA][0] < uiSettingsModeParameter[uiSLA][3])
    {
        piModeActors[uiSLA][1]=1;
    }

    if(piModeActors[uiSLA][1] == 1)
    {
        piModeActors[uiSLA][0]+=uiSettingsModeParameter[uiSLA][4];
    }
    else
    {
        piModeActors[uiSLA][0]-=uiSettingsModeParameter[uiSLA][3];
    }

    vGraphicsSetPixelFromTo(1<<uiSLA,piModeActors[uiSLA][0],250-piModeActors[uiSLA][0],puiColor);

}

