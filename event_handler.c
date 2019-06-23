#include "event_handler.h"
#include "makros.h"
#include "settings.h"

#include <time.h>

double puiEventHandlerActor[VIRTUAL_SLA_NUMBER][EVENT_HANDLER_NUMBER];

void vEventHandlerInit(void)
{
	
}

void vEventHandlerCheckEvents(void)
{
	char uiVirtualSLA, uiEventHandler;
	
	for(uiVirtualSLA=0;uiVirtualSLA < VIRTUAL_SLA_NUMBER;uiVirtualSLA++)
	{
		for(uiEventHandler=0;uiEventHandler < EVENT_HANDLER_NUMBER;uiEventHandler++)
		{
			vEventHandler(uiVirtualSLA,uiEventHandler);
		}
	}
}

void vEventHandler(char uiVirtualSLA, char uiEventHandler)
{
	switch(uiEventHandler)
	{
		case 0: vEventHandler0(uiVirtualSLA);	//Alarm
		break;
		
	}
}

/*
Parameters:
0 - Mode
1 - Hour
2 - Minute
3 - Second
*/
void vEventHandler0(char uiVirtualSLA)
{	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if(tsSettingsEventHandler[uiVirtualSLA][0].bEnabled && !tsSettingsEventHandler[uiVirtualSLA][0].bIsActive)
	{
		if(tm.tm_hour == tsSettingsEventHandler[uiVirtualSLA][0].uiParameter[0] && tm.tm_min == tsSettingsEventHandler[uiVirtualSLA][0].uiParameter[1])
		{
			uiSettingsModeActive[uiVirtualSLA] = tsSettingsEventHandler[uiVirtualSLA][0].uiParameter[0];
			
			uiSettingsModeParameter[uiVirtualSLA][0]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[0];
			uiSettingsModeParameter[uiVirtualSLA][1]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[1];
			uiSettingsModeParameter[uiVirtualSLA][2]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[2];
			uiSettingsModeParameter[uiVirtualSLA][3]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[3];
			uiSettingsModeParameter[uiVirtualSLA][4]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[4];
			uiSettingsModeParameter[uiVirtualSLA][5]=tsSettingsEventHandler[uiVirtualSLA][0].uiModeParameter[5];
			
			tsSettingsEventHandler[uiVirtualSLA][0].bIsActive = 1;
		}
	}
	else
	{
		tsSettingsEventHandler[uiVirtualSLA][0].bEnabled = 0;
		tsSettingsEventHandler[uiVirtualSLA][0].bIsActive = 0;
	}
}