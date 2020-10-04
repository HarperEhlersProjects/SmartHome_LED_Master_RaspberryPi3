#include "led_serial.h"


#include "framebuffer.h"
#include "system.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

Transmitter::Transmitter(System* system)
{
    int iUART0_filestream = -1, uiCount;

	this->system = system;

    iUART0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(iUART0_filestream == -1)
    {
        printf("Error - Unable to open UART!");
    }

    struct termios options;
    tcgetattr(iUART0_filestream,&options);				//get current options
    options.c_cflag = B4000000 | CS8 | CLOCAL | CREAD;  //Set baud rate to 4MHz
    options.c_iflag = IGNPAR;							//No parity
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(iUART0_filestream, TCIOFLUSH);				//clear filestream
    tcsetattr(iUART0_filestream, TCSANOW, &options);	//apply modified options

    iPort = iUART0_filestream;
}

void Transmitter::vSerialTransmit()
{
    char uiTempTx = 'r', uiTempRx = 0;
    int iCount, iRet;
	bool transmissionComplete = false;

	//while transmission was not completed successfully
    while(!transmissionComplete)
    {   
		transmissionComplete = false;
        uiTempRx = 0;
        read(iPort, &uiTempRx, 1);
        
		//When data receiver signals its ready state
        if(uiTempRx == 'r')
        {
            printf("uC is ready\n");   
            tcflush(iPort, TCIFLUSH);

			//Transmit the ready state of transmitter
            write(iPort, &uiTempTx, 1);
            printf("Pi is ready\n");

			//Wait for start signal of the data receiver
            uiTempRx = 0;
            while(read(iPort, &uiTempRx, 1) != 1);

			//When Receiver signals the transmitter to start the transmission
            if(uiTempRx == 's')
            {
                printf("Transmission start granted\n");
                tcflush(iPort, TCIOFLUSH);
                
				//Loop data transmit until all data is sent
                iCount = 0;
                while(iCount < LED_SERIAL_DATA_SIZE)
                {   
					//Write data to uart filestream and count written bytes.
                    iRet = write(iPort, &(uiLEDSerialBuffer[iCount]), LED_SERIAL_DATA_SIZE-iCount);
                    if(iRet != -1)
                    {
                        iCount += iRet;
                    }
                }
                
				//transmission completed
				transmissionComplete = true;
                printf("Transmission end\n\n");
            }
            else
            {
                tcflush(iPort, TCIOFLUSH);
            }
        }
		//When receiver did not receive expected number of bytes in the previous transmission
        else if(uiTempRx == 'f')
        {
            printf("Transmission failed\n\n");
        }
    }

    return;
}

void Transmitter::vRGB2PacketSerial(void)
{
	long i, uiLEDOffset, uiLED, uiVirtualLED, uiVirtualSLA, uiSegmentCounter;
	char uiSLAMask;
	
	//Iterate through all virtual pixel array objects
	for(uiVirtualSLA=0;uiVirtualSLA < this->system->virtualSLAs.size();uiVirtualSLA++)
	{
		//Iterate through all segments of current pixel array object
		for(uiSegmentCounter=0;uiSegmentCounter < this->system->virtualSLAs[uiVirtualSLA]->segments.size();uiSegmentCounter++)
		{
			//When current segment has a configured destination physical pixel array
			if(system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].uiDestSLA != VIRTUAL_SLA_DEST_NONE)
			{
				//Calculate the corresponding mask of the destination physical pixel array object
				uiSLAMask = 1 << system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].uiDestSLA;
				//Whether the mapping direction is inverted or not
				if(system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].bSLAInverted)
				{	
					//Iterating through every pixel of the current segment and copying it to the designated pixel of destination physical pixel array
					for(uiLEDOffset=0;uiLEDOffset < system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].firstDestLED - uiLEDOffset;	//Inverted direction
						
						if(uiLED >= 0)
						{
							vSetPixel(uiSLAMask, uiLED, uiVirtualSLA, uiVirtualLED, system->SLAs[system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].uiDestSLA].colorType);
						}
					}
				}
				else
				{
					//Iterating through every pixel of the current segment and copying it to the designated pixel of destination physical pixel array
					for(uiLEDOffset=0;uiLEDOffset < system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].firstDestLED + uiLEDOffset;	//Not inverted direction
					
						vSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED, system->SLAs[system->virtualSLAs[uiVirtualSLA]->segments[uiSegmentCounter].uiDestSLA].colorType);
					}
				}
			}
		}
	}
}


/*
Write a pixel for a specified physical pixel array according to a specific format.
In the following schematic is described which Bit refers to which physical LED-Array, which LED, which color and which Bit of this 8 Bit coded color.
																	First LED of all physical LED-Arrays														|Second LED of all physical LED-Arrays
Color and Bit-Index from LSB					->		  Red 0    Red 1    ...    Red 7   Green 0   Green 1   ...   Green 7   Blue 0   Blue 1    ...	 Blue 7	| Red 0		...
Bytes of buffer with MSB------LSB devided by |	->		--------|--------|  ...  |--------|--------|--------|  ...  |--------|--------|--------|  ...  |--------|--------|	...
Physical LED-Array No.							->		76543210	""      ...      ""       ""       ""      ...      ""       ""       ""	  ...	   ""	|	""		...
*/
void Transmitter::vSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED,char uiSLAType)
{
	char i;
	
	//Iterating through 8 Bits of each color intensity of current pixel
	for(i=0;i<8;i++)
	{
		//Set the corresponding bits for every color in the serial buffer according to the value of the current bits.
		switch(uiSLAType)
		{
			case SETTINGS_SLATYPE_RGB:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GBR:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BRG:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BGR:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GRB:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_RBG:
				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA]->framebuffer.getPixel(uiVirtualLED).uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
		}
	}	
}
