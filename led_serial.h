#ifndef LED_SERIAL_H
#define LED_SERIAL_H

#include "makros.h"

char puiLEDSerialData[LED_SERIAL_DATA_SIZE];

void vLEDSerialTransmitterInit(void);
void vLEDSerialTransmit(void);

#endif /* LED_SERIAL_H */
