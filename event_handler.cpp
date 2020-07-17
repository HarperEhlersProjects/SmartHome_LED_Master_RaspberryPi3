//#include "event_handler.h"
//
//#include "makros.h"
//#include "system.h"
//
//#include <time.h>
//
//double puiEventHandlerActor[VIRTUAL_SLA_NUMBER][EVENT_HANDLER_NUMBER];
//
//void vEventHandlerInit(void)
//{
//	
//}
//
//void vEventHandlerCheckEvents(void)
//{
//	char uiVirtualSLA, uiEventHandler;
//	
//	for(uiVirtualSLA=0;uiVirtualSLA < VIRTUAL_SLA_NUMBER;uiVirtualSLA++)
//	{
//		for(uiEventHandler=0;uiEventHandler < EVENT_HANDLER_NUMBER;uiEventHandler++)
//		{
//			vEventHandler(uiVirtualSLA,uiEventHandler);
//		}
//	}
//}
//
//void vEventHandler(char uiVirtualSLA, char uiEventHandler)
//{
//	switch(uiEventHandler)
//	{
//		case 0: vEventHandler0(uiVirtualSLA);	//Alarm
//		break;
//		
//	}
//}
//
///*
//Parameters:
//0 - Mode
//1 - Hour
//2 - Minute
//3 - Second
//*/
//void vEventHandler0(char uiVirtualSLA)
//{	
//	time_t t = time(NULL);
//	struct tm tm = *localtime(&t);
//
//	if(tsSettingsEventHandler[uiVirtualSLA].bEnabled && !tsSettingsEventHandler[uiVirtualSLA].bIsActive)
//	{
//		if(tm.tm_hour == tsSettingsEventHandler[uiVirtualSLA].uiParameter[1] && tm.tm_min == tsSettingsEventHandler[uiVirtualSLA].uiParameter[2])
//		{
//			uiSettingsModeActive[uiVirtualSLA] = tsSettingsEventHandler[uiVirtualSLA].uiParameter[0];
//			
//			uiSettingsModeParameter[uiVirtualSLA][0]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[0];
//			uiSettingsModeParameter[uiVirtualSLA][1]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[1];
//			uiSettingsModeParameter[uiVirtualSLA][2]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[2];
//			uiSettingsModeParameter[uiVirtualSLA][3]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[3];
//			uiSettingsModeParameter[uiVirtualSLA][4]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[4];
//			uiSettingsModeParameter[uiVirtualSLA][5]=tsSettingsEventHandler[uiVirtualSLA].uiModeParameter[5];
//			
//			tsSettingsEventHandler[uiVirtualSLA].bIsActive = 1;
//		}
//	}
//	else if(tsSettingsEventHandler[uiVirtualSLA].bIsActive)
//	{
//		tsSettingsEventHandler[uiVirtualSLA].bEnabled = 0;
//		tsSettingsEventHandler[uiVirtualSLA].bIsActive = 0;
//	}
//}