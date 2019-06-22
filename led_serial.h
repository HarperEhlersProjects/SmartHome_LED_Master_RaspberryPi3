#ifndef LED_SERIAL_H
#define LED_SERIAL_H

#include "makros.h"

void vLEDSerialTransmitterInit(void);
void vLEDSerialTransmit(void);
void vLEDSerialRGB2PacketSerial(void);
void vLEDSerialSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED,char uiSLAType);
#endif /* LED_SERIAL_H */
