#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "graphics.h"
#include "led_serial.h"

#include "settings.h"

int iPort;

char uiLEDSerialBuffer[LED_SERIAL_DATA_SIZE];

void vLEDSerialTransmitterInit(void)
{
    int iUART0_filestream = -1, uiCount;

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

void vLEDSerialTransmit()
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

void vLEDSerialRGB2PacketSerial(void)
{
	long i, uiLEDOffset, uiLED, uiVirtualLED, uiVirtualSLA, uiSegmentCounter;
	char uiSLAMask;
	
	for(uiVirtualSLA=0;uiVirtualSLA < VIRTUAL_SLA_NUMBER;uiVirtualSLA++)
	{
		for(uiSegmentCounter=0;uiSegmentCounter < VIRTUAL_SLA_SEGMENTS_NUMBER;uiSegmentCounter++)
		{
			if(tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestSLA != VIRTUAL_SLA_DEST_NONE)
			{
				uiSLAMask = 1 << tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestSLA;
				if(tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].bSLAInverted)
				{	
					for(uiLEDOffset=0;uiLEDOffset < tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiSegmentLength;uiLEDOffset++)
					{
						uiVirtualLED = tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiSourceLEDStart + uiLEDOffset;
						uiLED = tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestLEDStart - uiLEDOffset;
						
						vLEDSerialSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED,uiSettingsSLAType[tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestSLA]);
					}
				}
				else
				{
					for(uiLEDOffset=0;uiLEDOffset < tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiSegmentLength;uiLEDOffset++)
					{
						uiVirtualLED = tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiSourceLEDStart + uiLEDOffset;
						uiLED = tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestLEDStart + uiLEDOffset;
					
						vLEDSerialSetPixel(uiSLAMask,uiLED,uiVirtualSLA,uiVirtualLED,uiSettingsSLAType[tsSettingsVirtualSLAMap[uiVirtualSLA][uiSegmentCounter].uiDestSLA]);
						
					}
				}
			}
		}
	}
}

void vLEDSerialSetPixel(char uiSLAMask, long uiLED, long uiVirtualSLA, long uiVirtualLED,char uiSLAType)
{
	char i;
	
	for(i=0;i<8;i++)
	{
		switch(uiSLAType)
		{
			case SETTINGS_SLATYPE_RGB:
				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GBR:
				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BRG:
				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_BGR:
				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 16 + i] &= ~uiSLAMask;
				}
			break;
			case SETTINGS_SLATYPE_GRB:
				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiGreen & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + i] &= ~uiSLAMask;
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiRed & (0x80 >> i)) > 0)
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] |= uiSLAMask;
				}
				else
				{
					uiLEDSerialBuffer[uiLED * 24 + 8 + i] &= ~uiSLAMask;        
				}

				if((puiGraphicsData[uiVirtualSLA][uiVirtualLED].uiBlue & (0x80 >> i)) > 0)
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
