#include "settings.h"
#include "led_serial.h"
#include "graphics.h"
#include "ui.h"
#include "modes.h"

#include "makros.h"
#include <time.h>

#include "LED_station.h"


void main(int argc, char **argv)
{  
    int time_previous,time_now;    

	vSettingsInit();
	vGraphicsInit();
	vModesInit();
	vLEDSerialTransmitterInit();

	//SLA Length
	uiSettingsSLALength[1] = 300;
	//Enable SLA
	uiSettingsSLAState[1] = SLA_ACTIVE;
	//Mode
    uiSettingsModeActive[1] = 0;
	//Huel
    uiSettingsModeParameter[1][0] = 0;
	//Saturation
    uiSettingsModeParameter[1][1] = 1;
	//Brightness
    uiSettingsModeParameter[1][2] = 0.5;
	//Gamme Enable
	uiSettingsModeParameter[1][3] = TRUE;
	
	
	
    uiSettingsModeParameter[1][4] = 1;
    uiSettingsModeParameter[1][5] = 0;
	

    while(1)
    {       
		vModesFrameCalculate();
		//while(15ms);
        vLEDSerialTransmit();

        vGraphicsPixelResetAll();
    }
}
