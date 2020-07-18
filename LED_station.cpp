#include "led_serial.h"
//#include "event_handler.h"
#include "system.h"
#include "mode_manager.h"
#include "settings.h"


#include "makros.h"
#include <time.h>


int main(int argc, char** argv)
{
	int time_previous, time_now;

	System system = System();
	Settings settings = Settings(&system);
	ModeManager modeManager = ModeManager(system.virtualSLAs);
	Transmitter transmitter = Transmitter(system.virtualSLAs, system.SLAs);
	
	settings.downSync();

	//Configure	SLAs
	system.SLAs[1].state = SLA_ACTIVE;

	//Configure Virtual SLA
	//VSLA-0:
	//Segment
	system.virtualSLAs[0].segments[0].uiDestSLA = 1;
	system.virtualSLAs[0].segments[0].length = 100;
	system.virtualSLAs[0].segments[0].firstDestLED = 0;
	system.virtualSLAs[0].segments[0].firstSourceLED = 0;

	//SLA Length
	system.virtualSLAs[0].length = 100;
	//Mode
	system.virtualSLAs[0].mode.Number = 4;
	//Huel
	system.virtualSLAs[0].mode.Parameter[0] = 0;
	//Saturation
	system.virtualSLAs[0].mode.Parameter[1] = 0.9;
	//Brightness
	system.virtualSLAs[0].mode.Parameter[2] = 0.9;
	//Gamme Enable
	system.virtualSLAs[0].mode.Parameter[3] = TRUE;
	
		
	system.virtualSLAs[0].mode.Parameter[4] = 1;
	system.virtualSLAs[0].mode.Parameter[5] = 0;
	
	
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
		//vEventHandlerCheckEvents();
		modeManager.vFrameCalculate();
		transmitter.vRGB2PacketSerial();
		//while(15ms);
        transmitter.vSerialTransmit();
    }

	return 0;
}
