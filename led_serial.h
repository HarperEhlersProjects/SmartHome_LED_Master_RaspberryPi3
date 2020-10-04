/*
The led_serial module provides a Transmitter which is capable of serializing rgb data of the virtual led arrays of a given system 
and transmitting them by using uart with a maximum of a given framerate.
*/

#ifndef LED_SERIAL_H
#define LED_SERIAL_H

#include "makros.h"
#include "system.h"


/*
Maps rgb data of virtual led arrays of a given system to physical led arrays and serializing the rgb data of physical led array efficiently in one step.
After serialized data is transmitted by using the uart controller of raspberry pi 3.
*/
class Transmitter
{
public:

	Transmitter(System* system);

	void vSerialTransmit(void);		//Transmits serialized and packed data in uiLEDSerialBuffer using uart
	void vRGB2PacketSerial(void);

private:

	/*
		serial buffer of rgb data for every physical pixel array. For simultan transmission to the physical LED-Arrays the data for all arrays is packed in a specific format. 
		In the following schematic is described which Bit refers to which physical LED-Array, which LED, which color and which Bit of this 8 Bit coded color.
																			First LED of all physical LED-Arrays														|Second LED of all physical LED-Arrays
		Color and Bit-Index from LSB					->		  Red 0    Red 1    ...    Red 7   Green 0   Green 1   ...   Green 7   Blue 0   Blue 1    ...	 Blue 7	| Red 0		...
		Bytes of buffer with MSB------LSB devided by |	->		--------|--------|  ...  |--------|--------|--------|  ...  |--------|--------|--------|  ...  |--------|--------|	...
		Physical LED-Array No.							->		76543210	""      ...      ""       ""       ""      ...      ""       ""       ""	  ...	   ""	|	""		...
	*/
	char uiLEDSerialBuffer[LED_SERIAL_DATA_SIZE] = {0};	//buffer of serialized and packed rgb data.
	int iPort;											//filestream which is linked to serial uart output.

	System* system;

	void vSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED, char uiSLAType);
};

#endif /* LED_SERIAL_H */
