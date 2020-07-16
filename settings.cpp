#include "settings.h"

//#include "cJSON.h"

//Definitions
long uiSettingsVirtualSLALength[VIRTUAL_SLA_NUMBER] = {};
char uiSettingsModeActive[VIRTUAL_SLA_NUMBER] = {};
double uiSettingsModeParameter[VIRTUAL_SLA_NUMBER][MODE_PARAMETER_NUMBER] = {};
tsVirtualSLAMap tsSettingsVirtualSLAMap[VIRTUAL_SLA_NUMBER][VIRTUAL_SLA_SEGMENTS_NUMBER] = {};

long uiSettingsSLALengthMAX = 0;

char uiSettingsSLAType[SLA_NUMBER] = {};
long uiSettingsSLALength[SLA_NUMBER] = {};
char uiSettingsSLAState[SLA_NUMBER] = {};

float fSettingsGamma8RedValue[VIRTUAL_SLA_NUMBER] = {};
float fSettingsGamma8GreenValue[VIRTUAL_SLA_NUMBER] = {};
float fSettingsGamma8BlueValue[VIRTUAL_SLA_NUMBER] = {};

extern tsEventHandler tsSettingsEventHandler[EVENT_HANDLER_NUMBER] = {};

void vSettingsInit(void)
{
	char i,j;
	for(i=0;i<SLA_NUMBER;i++)
	{
		uiSettingsSLALength[i] = 0;
		uiSettingsSLAState[i] = SLA_DEACTIVE;
	}
	
	
	for(i=0;i<VIRTUAL_SLA_NUMBER;i++)
	{
		uiSettingsVirtualSLALength[i] = 0;
		uiSettingsModeActive[i] = 0;
		
		for(j=0;j<MODE_PARAMETER_NUMBER;j++)
		{
			uiSettingsModeParameter[i][j] = 0;
		}
		
		for(j=0;j<VIRTUAL_SLA_SEGMENTS_NUMBER;j++)
		{
			tsSettingsVirtualSLAMap[i][j].uiDestSLA = VIRTUAL_SLA_DEST_NONE;
			tsSettingsVirtualSLAMap[i][j].bSLAInverted=FALSE;
			tsSettingsVirtualSLAMap[i][j].uiSegmentLength=0;
			tsSettingsVirtualSLAMap[i][j].uiSourceLEDStart=0;
			tsSettingsVirtualSLAMap[i][j].uiDestLEDStart=0;
		}
		
		uiSettingsSLAType[i] = SETTINGS_SLATYPE_RGB;
		
		fSettingsGamma8RedValue[i] = SETTINGS_GAMMA8_RED_DEFAULT;
		fSettingsGamma8GreenValue[i] = SETTINGS_GAMMA8_GREEN_DEFAULT;
		fSettingsGamma8BlueValue[i] = SETTINGS_GAMMA8_BLUE_DEFAULT;
	}
}