#include "led_serial.h"
//#include "event_handler.h"
#include "system.h"
#include "mode_manager.h"
#include "settings.h"
#include "game_box.h"

#include "makros.h"
#include <time.h>

#include <iostream>

int main(int argc, char** argv)
{
	int time_previous, time_now;

	System system = System();
	Settings settings = Settings(&system);
	ModeManager modeManager = ModeManager(&system);
	Transmitter transmitter = Transmitter(system.virtualSLAs, system.SLAs);

	settings.downSync();
	

	
    while(1)
    {   
		//vEventHandlerCheckEvents();
		settings.upSync();

		system.gamebox.calculateStep();

		modeManager.vFrameCalculate();
		transmitter.vRGB2PacketSerial();
		//while(15ms);
        transmitter.vSerialTransmit();



    }

	return 0;
}
