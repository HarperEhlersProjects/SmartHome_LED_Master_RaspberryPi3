#include "led_serial.h"


#include "graphics.h"
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
					//Write data to uart filestream and count written bytes
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
	
	for(uiVirtualSLA=0;uiVirtualSLA < VIRTUAL_SLA_NUMBER;uiVirtualSLA++)
	{
		for(uiSegmentCounter=0;uiSegmentCounter < VIRTUAL_SLA_SEGMENTS_NUMBER;uiSegmentCounter++)
		{
			if(system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA != VIRTUAL_SLA_DEST_NONE)
			{
				uiSLAMask = 1 << system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA;
				if(system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].bSLAInverted)
				{	
					for(uiLEDOffset=0;uiLEDOffset < system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstDestLED - uiLEDOffset;
						
						vSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED, system->SLAs[system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA].colorType);
					}
				}
				else
				{
					for(uiLEDOffset=0;uiLEDOffset < system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstDestLED + uiLEDOffset;
					
						vSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED, system->SLAs[system->virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA].colorType);
						
					}
				}
			}
		}
	}
}

void Transmitter::vSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED,char uiSLAType)
{
	char i;
	
	for(i=0;i<8;i++)
	{
		switch(uiSLAType)
		{
			case SETTINGS_SLATYPE_RGB:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GBR:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BRG:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BGR:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GRB:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_RBG:
				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((system->virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
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
