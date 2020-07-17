#ifndef LED_SERIAL_H
#define LED_SERIAL_H

#include "makros.h"
#include "system.h"

class Transmitter
{
public:
	char uiLEDSerialBuffer[LED_SERIAL_DATA_SIZE];
	int iPort;
	VirtualSLA* virtualSLAs;
	SLA* SLAs;

	Transmitter(VirtualSLA* virtualSLAs, SLA* SLAs);

	void vSerialTransmit(void);
	void vRGB2PacketSerial(void);
	void vSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED, char uiSLAType);
};

#endif /* LED_SERIAL_H */
