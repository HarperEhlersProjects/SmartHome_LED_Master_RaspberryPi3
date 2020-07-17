#include "led_serial.h"


#include "graphics.h"
#include "system.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>



Transmitter::Transmitter(VirtualSLA* virtualSLAs, SLA* SLAs)
{
    int iUART0_filestream = -1, uiCount;

	this->virtualSLAs = virtualSLAs;
	this->SLAs = SLAs;

    iUART0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(iUART0_filestream == -1)
    {
        printf("Error - Unable to open UART!");
    }

    struct termios options;
    tcgetattr(iUART0_filestream,&options);
    options.c_cflag = B4000000 | CS8 | CLOCAL | CREAD;   //Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(iUART0_filestream, TCIOFLUSH);
    tcsetattr(iUART0_filestream, TCSANOW, &options);

    iPort = iUART0_filestream;
	
	return;
}

void Transmitter::vSerialTransmit()
{
    char uiTempTx = 'r', uiTempRx = 0;
    int iCount, iRet;
    while(uiTempRx != 'c')
    {   
        uiTempRx = 0;
        read(iPort, &uiTempRx, 1);
        
        if(uiTempRx == 'r')
        {
            printf("uC is ready\n");   
            tcflush(iPort, TCIFLUSH);
            write(iPort, &uiTempTx, 1);
            printf("Pi is ready\n");

            uiTempRx = 0;
            while(read(iPort, &uiTempRx, 1) != 1);

            if(uiTempRx == 's')
            {
                printf("Transmission start granted\n");
                tcflush(iPort, TCIOFLUSH);
                
                iCount = 0;
                while(iCount < LED_SERIAL_DATA_SIZE)
                {   
                    iRet = write(iPort, &(uiLEDSerialBuffer[iCount]), LED_SERIAL_DATA_SIZE-iCount);
                    if(iRet != -1)
                    {
                        iCount += iRet;
                    }
                    //printf("%d\n", iCount);
                }
                
                uiTempRx = 'c';
                printf("Transmission end\n\n");
            }
            else
            {
                tcflush(iPort, TCIOFLUSH);
            }
        }
        else if(uiTempRx == 'f')
        {
            printf("Transmission failed\n\n");
            //tcflush(iPort, TCIOFLUSH);
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
			if(virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA != VIRTUAL_SLA_DEST_NONE)
			{
				uiSLAMask = 1 << virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA;
				if(virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].bSLAInverted)
				{	
					for(uiLEDOffset=0;uiLEDOffset < virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstDestLED - uiLEDOffset;
						
						vSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED,SLAs[virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA].colorType);
					}
				}
				else
				{
					for(uiLEDOffset=0;uiLEDOffset < virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].length;uiLEDOffset++)
					{
						uiVirtualLED = virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstSourceLED + uiLEDOffset;
						uiLED = virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].firstDestLED + uiLEDOffset;
					
						vSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED, SLAs[virtualSLAs[uiVirtualSLA].segments[uiSegmentCounter].uiDestSLA].colorType);
						
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
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GBR:
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BRG:
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BGR:
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GRB:
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_RBG:
				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((virtualSLAs[uiVirtualSLA].graphics.frameBuffer[uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
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
