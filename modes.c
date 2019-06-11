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
			case 4:vMode4(uiCounterSLA);//Color Fade
		}
	}
}

void vMode0(char uiSLA)
{

}


/*
parameter 0-2: RGB: 0-255
parameter 3: GammaorrectionEnable: boolean
*/
void vMode1(char uiSLA)
{
	tsGraphicsHSV tsColorHSV;
	tsGraphicsRGB tsColorRGB;
    long uiCounter;
	
	tsColorHSV.uiHuel = uiSettingsModeParameter[uiSLA][0];
	tsColorHSV.udSaturation = uiSettingsModeParameter[uiSLA][1];
	tsColorHSV.udBrightness = uiSettingsModeParameter[uiSLA][2];
	
	tsColorRGB = tsGraphicsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if(uiSettingsModeParameter[uiSLA][3])
	{
		tsColorRGB = tsGraphicsGamma8Correction(uiSLA, tsColorRGB);
	}
	
    for(uiCounter=0;uiCounter<uiSettingsSLALength[uiSLA];uiCounter++)
    {
        vGraphicsSetPixel(1<<uiSLA,uiCounter,tsColorRGB);
    }
}

/*
parameter 0-2: RGB: 0-255
parameter 3: GammaorrectionEnable: boolean
parameter 4: snake length: 0-255
parameter 5: snake veloity: 0-255
*/
void vMode2(char uiSLA)
{
	tsGraphicsHSV tsColorHSV;
    tsGraphicsRGB tsColorRGB;
    long uiCounter;
    long uiTopBoundary,uiBottomBoundary;

	tsColorHSV.uiHuel = uiSettingsModeParameter[uiSLA][0];
	tsColorHSV.udSaturation = uiSettingsModeParameter[uiSLA][1];
	tsColorHSV.udBrightness = uiSettingsModeParameter[uiSLA][2];
	
	tsColorRGB = tsGraphicsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if(uiSettingsModeParameter[uiSLA][3])
	{
		tsColorRGB = tsGraphicsGamma8Correction(uiSLA, tsColorRGB);
	}

    piModeActors[uiSLA][1]++;

    if(piModeActors[uiSLA][1] > 255-uiSettingsModeParameter[uiSLA][5])
    {
        piModeActors[uiSLA][0]++;

        if(piModeActors[uiSLA][0] >= uiSettingsSLALength[uiSLA] + uiSettingsModeParameter[uiSLA][4])
        {
            piModeActors[uiSLA][0] = 0;
        }

        piModeActors[uiSLA][1] = 0;
    }

    if(piModeActors[uiSLA][0] - uiSettingsModeParameter[uiSLA][4] < 0)  //cut if its too long for SLA
    {
        uiBottomBoundary = 0;
    }
    else
    {
        uiBottomBoundary = piModeActors[uiSLA][0] - uiSettingsModeParameter[uiSLA][4];
    }

    uiTopBoundary=piModeActors[uiSLA][0];

    if(uiTopBoundary >= uiSettingsSLALength[uiSLA])
    {
        uiTopBoundary = uiSettingsSLALength[uiSLA];

    }

    for(uiCounter = uiBottomBoundary;uiCounter < uiTopBoundary;uiCounter++)
    {
        vGraphicsSetPixel(1<<uiSLA,uiCounter,tsColorRGB);
    }
}

/*
parameter 0-2: RGB_influence: 0-255
parameter 3: GammaorrectionEnable: boolean
parameter 4: ShrinkingVelocity: 0-255
parameter 5: expanding velocity: 0-255
*/

void vMode3(char uiSLA)
{
	tsGraphicsHSV tsColorHSV;
    tsGraphicsRGB tsColorRGB;

	tsColorHSV.uiHuel = uiSettingsModeParameter[uiSLA][0];
	tsColorHSV.udSaturation = uiSettingsModeParameter[uiSLA][1];
	tsColorHSV.udBrightness = uiSettingsModeParameter[uiSLA][2];
	
	tsColorRGB = tsGraphicsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if(uiSettingsModeParameter[uiSLA][3])
	{
		tsColorRGB = tsGraphicsGamma8Correction(uiSLA, tsColorRGB);
	}
	
    if(piModeActors[uiSLA][0] > uiSettingsSLALength[uiSLA]/2)
    {   
        piModeActors[uiSLA][1]=0;
    }
    else if(piModeActors[uiSLA][0] < uiSettingsModeParameter[uiSLA][4])
    {
        piModeActors[uiSLA][1]=1;
    }

    if(piModeActors[uiSLA][1] == 1)
    {
        piModeActors[uiSLA][0]+=uiSettingsModeParameter[uiSLA][5];
    }
    else
    {
        piModeActors[uiSLA][0]-=uiSettingsModeParameter[uiSLA][4];
    }

    vGraphicsSetPixelFromTo(1<<uiSLA,piModeActors[uiSLA][0],250-piModeActors[uiSLA][0],tsColorRGB);

}

//Color Fade
void vMode4(char uiSLA)
{
	tsGraphicsHSV tsColorHSV;
	tsGraphicsRGB tsColorRGB;
    long uiCounter;
	
	for(uiCounter=0;uiCounter<uiSettingsSLALength[uiSLA];uiCounter++)
    {
	
	tsColorHSV.uiHuel = 360*uiCounter/uiSettingsSLALength[uiSLA];
	tsColorHSV.udSaturation = uiSettingsModeParameter[uiSLA][1];
	tsColorHSV.udBrightness = uiSettingsModeParameter[uiSLA][2];
	
	tsColorRGB = tsGraphicsHSV2RGB(tsColorHSV);
	
		//use gamma correction if enabled
		if(uiSettingsModeParameter[uiSLA][3])
		{
			tsColorRGB = tsGraphicsGamma8Correction(uiSLA, tsColorRGB);
		}
		
		if(uiCounter + piModeActors[uiSLA][1] >= uiSettingsSLALength[uiSLA])
		{
			vGraphicsSetPixel(1<<uiSLA,uiCounter + piModeActors[uiSLA][1] - uiSettingsSLALength[uiSLA],tsColorRGB);
		}
		else
		{
			vGraphicsSetPixel(1<<uiSLA,uiCounter + piModeActors[uiSLA][1],tsColorRGB);
		}

    }
		
	if(piModeActors[uiSLA][1] >= uiSettingsSLALength[uiSLA])
	{
		piModeActors[uiSLA][1] = 0;
	}
	else
	{	
		piModeActors[uiSLA][1] += uiSettingsModeParameter[uiSLA][4];
	}
}
