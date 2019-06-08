#include "settings.h"
#include "led_serial.h"
#include "graphics.h"
#include "ui.h"

#include <time.h>

#include "media_station.h"


void main(int argc, char **argv)
{  
    int time_previous,time_now;    

	vSettingsInit();
	vGraphicsInit();
	vLEDSerialTransmitterInit();

	/*
	uiSettingsSLALength[1] = 255;
	uiSettingsSLAState[1] = SLA_ACTIVE;	

    uiSettingsModeActive[1] = 3;
    uiSettingsModeParameter[1][0] = 1;
    uiSettingsModeParameter[1][1] = 2;
    uiSettingsModeParameter[1][2] = 0;
    
    uiSettingsModeParameter[1][3] = 10;
    uiSettingsModeParameter[1][4] = 3;
	*/

    while(1)
    {       
		vGraphicsFrameCalculate();
		//while(15ms);
        vLEDSerialTransmit();

        vGraphicsPixelResetAll();
    }
}
