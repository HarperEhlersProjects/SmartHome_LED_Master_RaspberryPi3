#include "mode_manager.h"

#include "game_box.h"
#include "graphics.h"


void ModeManager::vFrameCalculate()
{
	char uiCounterSLA;
	
	for(uiCounterSLA=0;uiCounterSLA < VIRTUAL_SLA_NUMBER;uiCounterSLA++)
	{
		VirtualSLA* currentVirtualSLA = &virtualSLAs[uiCounterSLA];

		currentVirtualSLA->graphics.vResetAllPixel();

		switch(currentVirtualSLA->mode.Number)
        {
			case 0:ModeManager::vMode0(currentVirtualSLA);//LED off
            break;
            case 1:ModeManager::vMode1(currentVirtualSLA);//constant RGB
            break;
            case 2:ModeManager::vMode2(currentVirtualSLA);//moving snake RGB
            break;
            case 3:ModeManager::vMode3(currentVirtualSLA);//Pulse
            break;
			case 4:ModeManager::vMode4(currentVirtualSLA);//Color Fade
			break;
			case 5:ModeManager::vMode5(currentVirtualSLA);//Game Box
		}
	}
}

void ModeManager::vMode0(VirtualSLA* vSLA)
{

}


/*
parameter 0-2: RGB: 0-255
parameter 3: GammaorrectionEnable: boolean
*/
void ModeManager::vMode1(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
	tsRGB tsColorRGB;
    long uiCounter;
	
	tsColorHSV.uiHuel = vSLA->mode.Parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
	tsColorRGB = GraphicsTB::tsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if(vSLA->mode.Parameter[3])
	{
		tsColorRGB = vSLA->graphics.tsGamma8Correction(tsColorRGB);
	}
	
    for(uiCounter=0;uiCounter< vSLA->length;uiCounter++)
    {
		vSLA->graphics.vSetPixel(uiCounter,tsColorRGB);
    }
}

/*
parameter 0-2: RGB: 0-255
parameter 3: GammaorrectionEnable: boolean
parameter 4: snake length: 0-255
parameter 5: snake veloity: 0-255
*/
void ModeManager::vMode2(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
    tsRGB tsColorRGB;
    long uiCounter;
    long uiTopBoundary,uiBottomBoundary;

	tsColorHSV.uiHuel = vSLA->mode.Parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
	tsColorRGB = GraphicsTB::tsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if (vSLA->mode.Parameter[3])
	{
		tsColorRGB = vSLA->graphics.tsGamma8Correction(tsColorRGB);
	}

	vSLA->mode.Actors[1]++;

    if(vSLA->mode.Actors[1] > 255- vSLA->mode.Parameter[5])
    {
		vSLA->mode.Actors[0]++;

        if(vSLA->mode.Actors[0] >= vSLA->length + vSLA->mode.Parameter[4])
        {
			vSLA->mode.Actors[0] = 0;
        }

		vSLA->mode.Actors[1] = 0;
    }

    if(vSLA->mode.Actors[0] - vSLA->mode.Parameter[4] < 0)  //cut if its too long for SLA
    {
        uiBottomBoundary = 0;
    }
    else
    {
        uiBottomBoundary = vSLA->mode.Actors[0] - vSLA->mode.Parameter[4];
    }

    uiTopBoundary = vSLA->mode.Actors[0];

    if(uiTopBoundary >= vSLA->length)
    {
        uiTopBoundary = vSLA->length;

    }

    for(uiCounter = uiBottomBoundary;uiCounter < uiTopBoundary;uiCounter++)
    {
		vSLA->graphics.vSetPixel(uiCounter,tsColorRGB);
    }
}

/*
parameter 0-2: RGB_influence: 0-255
parameter 3: GammaorrectionEnable: boolean
parameter 4: ShrinkingVelocity: 0-255
parameter 5: expanding velocity: 0-255
*/
void ModeManager::vMode3(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
    tsRGB tsColorRGB;

	tsColorHSV.uiHuel = vSLA->mode.Parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
	tsColorRGB = GraphicsTB::tsHSV2RGB(tsColorHSV);
	
	//use gamma correction if enabled
	if (vSLA->mode.Parameter[3])
	{
		tsColorRGB = vSLA->graphics.tsGamma8Correction(tsColorRGB);
	}
	
    if(vSLA->mode.Actors[0] > vSLA->length/2)
    {   
		vSLA->mode.Actors[1] = 0;
    }
    else if(vSLA->mode.Actors[0] < vSLA->mode.Parameter[4])
    {
		vSLA->mode.Actors[1] = 1;
    }

    if(vSLA->mode.Actors[1] == 1)
    {
		vSLA->mode.Actors[0] += vSLA->mode.Parameter[5];
    }
    else
    {
		vSLA->mode.Actors[0] -= vSLA->mode.Parameter[4];
    }

	vSLA->graphics.vSetPixelFromTo(vSLA->mode.Actors[0], vSLA->length -1- vSLA->mode.Actors[0],tsColorRGB);

}

//Color Fade
void ModeManager::vMode4(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
	tsRGB tsColorRGB;
    long uiCounter;
	
	for(uiCounter=0;uiCounter< vSLA->length;uiCounter++)
    {
	
	tsColorHSV.uiHuel = 360*uiCounter/ vSLA->length;
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
	tsColorRGB = GraphicsTB::tsHSV2RGB(tsColorHSV);
	
		//use gamma correction if enabled
		if (vSLA->mode.Parameter[3])
		{
			tsColorRGB = vSLA->graphics.tsGamma8Correction(tsColorRGB);
		}
		
		if(uiCounter + vSLA->mode.Actors[1] >= vSLA->length)
		{
			vSLA->graphics.vSetPixel(uiCounter + vSLA->mode.Actors[1] - vSLA->length,tsColorRGB);
		}
		else
		{
			vSLA->graphics.vSetPixel(uiCounter + vSLA->mode.Actors[1],tsColorRGB);
		}

    }
		
	if(vSLA->mode.Actors[1] >= vSLA->length)
	{
		vSLA->mode.Actors[1] = 0;
	}
	else
	{	
		vSLA->mode.Actors[1] += vSLA->mode.Parameter[4];
	}
}


//Game Box
void ModeManager::vMode5(VirtualSLA* vSLA)
{
	
	
}