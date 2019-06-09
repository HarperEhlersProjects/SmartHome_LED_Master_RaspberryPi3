#include "modes.h"
#include "makros.h"
#include "graphics.h"


#include "settings.h"

int piModeActors[SLA_NUMBER][MODE_ACTOR_NUMBER];

void vModesInit()
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

void vModesFrameCalculate()
{
	char uiCounterSLA;
	
	for(uiCounterSLA=0;uiCounterSLA < SLA_NUMBER;uiCounterSLA++)
	{
		switch(uiSettingsModeActive[uiCounterSLA])
        {
            case 0:vMode0(uiCounterSLA);//LED off
            break;
            case 1:vMode1(uiCounterSLA);//constant RGB
            break;
            case 2:vMode2(uiCounterSLA);//moving snake RGB
            break;
            case 3:vMode3(uiCounterSLA);//Pulse
            break;
		}
	}
}

void vMode0(char uiSLA)
{

}

void vMode1(char uiSLA)
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

void vMode2(char uiSLA)
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

void vMode3(char uiSLA)
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
