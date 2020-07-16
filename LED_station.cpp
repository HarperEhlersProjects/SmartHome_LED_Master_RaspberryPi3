#include "LED_station.h"

#include "led_serial.h"
#include "graphics.h"
#include "event_handler.h"
#include "settings.h"
#include "modes.h"
#include "ui.h"

#include "makros.h"
#include <time.h>


int main(int argc, char **argv)
{  
    int time_previous,time_now;

	vSettingsInit();
	vGraphicsInit();
	//vModesInit();
	ModeManager modeManager = new ModeManager;
	vEventHandlerInit();
	vLEDSerialTransmitterInit();
	

	//Configure	SLAs
	uiSettingsSLAType[0] = SETTINGS_SLATYPE_RGB;
	uiSettingsSLAType[1] = SETTINGS_SLATYPE_RGB;
	
	//Configure Virtual SLA
	//VSLA-0:
	//Segment 0:
	tsSettingsVirtualSLAMap[0][0].uiDestSLA = 1;
	//tsSettingsVirtualSLAMap[0][0].bSLAInverted = False; default
	tsSettingsVirtualSLAMap[0][0].uiSegmentLength = 100;
	tsSettingsVirtualSLAMap[0][0].uiSourceLEDStart = 0;
	tsSettingsVirtualSLAMap[0][0].uiDestLEDStart = 0;
	
	//SLA Length
	uiSettingsVirtualSLALength[0] = 100;
	//Mode
    uiSettingsModeActive[0] = 4;
	//Huel
    uiSettingsModeParameter[0][0] = 0;
	//Saturation
    uiSettingsModeParameter[0][1] = 0.9;
	//Brightness
    uiSettingsModeParameter[0][2] = 0.5;
	//Gamme Enable
	uiSettingsModeParameter[0][3] = TRUE;
	
		
    uiSettingsModeParameter[0][4] = 1;
    uiSettingsModeParameter[0][5] = 0;
	
	
	////Configure Event Handler
	//tsSettingsEventHandler[0].bEnabled = TRUE;
	//tsSettingsEventHandler[0].uiParameter[0] = 4;
	//tsSettingsEventHandler[0].uiParameter[1] = 4;
	//tsSettingsEventHandler[0].uiParameter[2] = 4;
	//tsSettingsEventHandler[0].uiParameter[3] = 4;
	//
	//tsSettingsEventHandler[0].uiModeParameter[0] = 0;
	//tsSettingsEventHandler[0].uiModeParameter[1] = 1;
	//tsSettingsEventHandler[0].uiModeParameter[2] = 0.5;
	//tsSettingsEventHandler[0].uiModeParameter[3] = TRUE;
	//tsSettingsEventHandler[0].uiModeParameter[4] = 1;
	//tsSettingsEventHandler[0].uiModeParameter[5] = 0;
	
    while(1)
    {   
		vEventHandlerCheckEvents();
		vModesFrameCalculate();
		vLEDSerialRGB2PacketSerial();
		//while(15ms);
        vLEDSerialTransmit();

        vGraphicsPixelResetAll();
    }

	return 0;
}
