#include "led_serial.h"
//#include "event_handler.h"
#include "system.h"
#include "mode_manager.h"
#include "settings.h"
#include "game_box.h"

#include "makros.h"
#include <time.h>


int main(int argc, char** argv)
{
	int time_previous, time_now;

	System system = System();
	Settings settings = Settings(&system);
	ModeManager modeManager = ModeManager(&system);
	Transmitter transmitter = Transmitter(system.virtualSLAs, system.SLAs);

	settings.downSync();
	
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
		settings.upSync();

		system.gamebox.CalculateStep();

		modeManager.vFrameCalculate();
		transmitter.vRGB2PacketSerial();
		//while(15ms);
        transmitter.vSerialTransmit();
    }

	return 0;
}
