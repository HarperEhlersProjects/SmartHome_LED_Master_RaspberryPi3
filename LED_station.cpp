
#include "system.h"

#include "led_serial.h"
//#include "event_handler.h"
#include "mode_manager.h"
#include "settings.h"

#include "makros.h"
#include <sys/time.h>
#include <iostream>


int main(int argc, char** argv)
{
	timeval time_before, time_now;
	float calculation_time_remain;

	System system = System();
	Settings settings(&system);
	ModeManager modeManager(&system);
	Transmitter transmitter(&system);

    while(true)
    {   
		//vEventHandlerCheckEvents();
		settings.Sync();

		system.gamebox.calculateStep();

		modeManager.vFrameCalculate();
		transmitter.vRGB2PacketSerial();

		//Toggle framerate to configured framerate in FRAME_PERIOD_MS
		gettimeofday(&time_now, nullptr);
		calculation_time_remain = FRAME_PERIOD_MS - ((time_now.tv_usec - time_before.tv_usec)/1000 + (time_now.tv_sec - time_before.tv_sec) * 1000);
		std::cout << "Remaining calculation time: " << calculation_time_remain << " ms" << std::endl;
		while(((time_now.tv_usec - time_before.tv_usec) / 1000 + (time_now.tv_sec - time_before.tv_sec) * 1000) < FRAME_PERIOD_MS){
			gettimeofday(&time_now,nullptr);
		}
		gettimeofday(&time_before, nullptr);

        transmitter.vSerialTransmit();
    }

	return 0;
}