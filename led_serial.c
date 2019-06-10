#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "graphics.h"
#include "led_serial.h"

int iPort;

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
                while(iCount < GRAPHICS_DATA_SIZE)
                {   
                    iRet = write(iPort, &(puiGraphicsData[iCount]), GRAPHICS_DATA_SIZE-iCount);
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