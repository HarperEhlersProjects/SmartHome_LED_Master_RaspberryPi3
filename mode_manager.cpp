#include "mode_manager.h"


void ModeManager::vFrameCalculate()
{
	char uiCounterSLA;
	
	for(uiCounterSLA=0;uiCounterSLA < VIRTUAL_SLA_NUMBER;uiCounterSLA++)
	{
		VirtualSLA* currentVirtualSLA = &(system->virtualSLAs[uiCounterSLA]);

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
*/
void ModeManager::vMode1(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
	tsRGB tsColorRGB;
    long uiCounter;
	
	tsColorHSV.uiHuel = vSLA->mode.Parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
    for(uiCounter=0;uiCounter< vSLA->length;uiCounter++)
    {
		vSLA->graphics.vSetPixel(uiCounter,tsColorHSV);
    }
}

/*
parameter 0-2: RGB: 0-255
parameter 3: snake length: 0-255
parameter 4: snake veloity: 0-255
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

	vSLA->mode.Actors[1]++;

    if(vSLA->mode.Actors[1] > 255- vSLA->mode.Parameter[4])
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
        uiBottomBoundary = vSLA->mode.Actors[0] - vSLA->mode.Parameter[3];
    }

    uiTopBoundary = vSLA->mode.Actors[0];

    if(uiTopBoundary >= vSLA->length)
    {
        uiTopBoundary = vSLA->length;

    }

    for(uiCounter = uiBottomBoundary;uiCounter < uiTopBoundary;uiCounter++)
    {
		vSLA->graphics.vSetPixel(uiCounter,tsColorHSV);
    }
}

/*
parameter 0-2: RGB_influence: 0-255
parameter 3: ShrinkingVelocity: 0-255
parameter 4: expanding velocity: 0-255
*/
void ModeManager::vMode3(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
    tsRGB tsColorRGB;

	tsColorHSV.uiHuel = vSLA->mode.Parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.Parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.Parameter[2];
	
    if(vSLA->mode.Actors[0] > vSLA->length/2)
    {   
		vSLA->mode.Actors[1] = 0;
    }
    else if(vSLA->mode.Actors[0] < vSLA->mode.Parameter[3])
    {
		vSLA->mode.Actors[1] = 1;
    }

    if(vSLA->mode.Actors[1] == 1)
    {
		vSLA->mode.Actors[0] += vSLA->mode.Parameter[4];
    }
    else
    {
		vSLA->mode.Actors[0] -= vSLA->mode.Parameter[3];
    }

	vSLA->graphics.vSetPixelFromTo(vSLA->mode.Actors[0], vSLA->length -1- vSLA->mode.Actors[0],tsColorHSV);

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
		
		if(uiCounter + vSLA->mode.Actors[1] >= vSLA->length)
		{
			vSLA->graphics.vSetPixel(uiCounter + vSLA->mode.Actors[1] - vSLA->length,tsColorHSV);
		}
		else
		{
			vSLA->graphics.vSetPixel(uiCounter + vSLA->mode.Actors[1],tsColorHSV);
		}

    }
		
	if(vSLA->mode.Actors[1] >= vSLA->length)
	{
		vSLA->mode.Actors[1] = 0;
	}
	else
	{	
		vSLA->mode.Actors[1] += vSLA->mode.Parameter[3];
	}
}


//Game Box
void ModeManager::vMode5(VirtualSLA* vSLA)
{
	system->gamebox.objects.drawObjects(&vSLA->display);
	vSLA->serializeDPUMatrix();
}