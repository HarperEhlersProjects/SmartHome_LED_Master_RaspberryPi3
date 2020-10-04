#include "mode_manager.h"

#include "display.h"
#include "color.h"



void ModeManager::vFrameCalculate()
{
	char uiCounterSLA;
	
	for(uiCounterSLA=0;uiCounterSLA < this->system->virtualSLAs.size();uiCounterSLA++)
	{
		VirtualSLA* currentVirtualSLA = (system->virtualSLAs[uiCounterSLA]);

		currentVirtualSLA->framebuffer.vResetAllPixel();

		switch(currentVirtualSLA->mode.number)
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
    long uiCounter;
	
	tsColorHSV.uiHuel = vSLA->mode.parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.parameter[2];
	
    for(uiCounter=0;uiCounter< vSLA->framebuffer.size();uiCounter++)
    {
		vSLA->framebuffer.vSetPixel(uiCounter,tsColorHSV);
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
    long uiCounter;
    long uiTopBoundary,uiBottomBoundary;

	tsColorHSV.uiHuel = vSLA->mode.parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.parameter[2];

	vSLA->mode.actors[1]++;

    if(vSLA->mode.actors[1] > 255- vSLA->mode.parameter[4])
    {
		vSLA->mode.actors[0]++;

        if(vSLA->mode.actors[0] >= vSLA->framebuffer.size() + vSLA->mode.parameter[4])
        {
			vSLA->mode.actors[0] = 0;
        }

		vSLA->mode.actors[1] = 0;
    }

    if(vSLA->mode.actors[0] - vSLA->mode.parameter[3] < 0)  //cut if its too long for SLA
    {
        uiBottomBoundary = 0;
    }
    else
    {
        uiBottomBoundary = vSLA->mode.actors[0] - vSLA->mode.parameter[3];
    }

    uiTopBoundary = vSLA->mode.actors[0];

    if(uiTopBoundary >= vSLA->framebuffer.size())
    {
        uiTopBoundary = vSLA->framebuffer.size();

    }

    for(uiCounter = uiBottomBoundary;uiCounter < uiTopBoundary;uiCounter++)
    {
		vSLA->framebuffer.vSetPixel(uiCounter,tsColorHSV);
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

	tsColorHSV.uiHuel = vSLA->mode.parameter[0];
	tsColorHSV.udSaturation = vSLA->mode.parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.parameter[2];
	
    if(vSLA->mode.actors[0] > vSLA->framebuffer.size() /2)
    {   
		vSLA->mode.actors[1] = 0;
    }
    else if(vSLA->mode.actors[0] < vSLA->mode.parameter[3])
    {
		vSLA->mode.actors[1] = 1;
    }

    if(vSLA->mode.actors[1] == 1)
    {
		vSLA->mode.actors[0] += vSLA->mode.parameter[4];
    }
    else
    {
		vSLA->mode.actors[0] -= vSLA->mode.parameter[3];
    }

	vSLA->framebuffer.vSetPixelFromTo(vSLA->mode.actors[0], vSLA->framebuffer.size() -1- vSLA->mode.actors[0],tsColorHSV);

}

//Color Fade
void ModeManager::vMode4(VirtualSLA* vSLA)
{
	tsHSV tsColorHSV;
    long uiCounter;
	
	for(uiCounter=0;uiCounter< vSLA->framebuffer.size();uiCounter++)
    {
	
	tsColorHSV.uiHuel = 360*uiCounter/ vSLA->framebuffer.size();
	tsColorHSV.udSaturation = vSLA->mode.parameter[1];
	tsColorHSV.udBrightness = vSLA->mode.parameter[2];
		
		if(uiCounter + vSLA->mode.actors[1] >= vSLA->framebuffer.size())
		{
			vSLA->framebuffer.vSetPixel(uiCounter + vSLA->mode.actors[1] - vSLA->framebuffer.size(),tsColorHSV);
		}
		else
		{
			vSLA->framebuffer.vSetPixel(uiCounter + vSLA->mode.actors[1],tsColorHSV);
		}

    }
		
	if(vSLA->mode.actors[1] >= vSLA->framebuffer.size())
	{
		vSLA->mode.actors[1] = 0;
	}
	else
	{	
		vSLA->mode.actors[1] += vSLA->mode.parameter[3];
	}
}


//Game Box
void ModeManager::vMode5(VirtualSLA* vSLA)
{
	system->gamebox.draw(&vSLA->display);
	vSLA->serializeDPUMatrix();
	vSLA->display.resetMatrix();
}