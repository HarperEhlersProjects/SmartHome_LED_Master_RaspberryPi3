
#include "system.h"

#include "led_serial.h"
//#include "event_handler.h"
#include "mode_manager.h"
#include "settings.h"

#include "makros.h"
#include <time.h>


int main(int argc, char** argv)
{
	int time_previous, time_now;

	System system = System();
	Settings settings(&system);
	ModeManager modeManager(&system);
	Transmitter transmitter(&system);

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
