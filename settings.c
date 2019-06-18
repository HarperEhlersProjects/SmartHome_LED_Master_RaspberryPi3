#include "settings.h"


void vSettingsInit(void)
{
	char i,j;
	for(i=0;i<SLA_NUMBER;i++)
	{
		uiSettingsSLALength[i] = 0;
		uiSettingsSLAState[i] = SLA_DEACTIVE;
		uiSettingsModeActive[i] = 0;
		
		for(j=0;j<MODE_PARAMETER_NUMBER;j++)
		{
			uiSettingsModeParameter[i][j] = 0;
		}
		
		fSettingsGamma8RedValue[i] = SETTINGS_GAMMA8_RED_DEFAULT;
		fSettingsGamma8GreenValue[i] = SETTINGS_GAMMA8_GREEN_DEFAULT;
		fSettingsGamma8BlueValue[i] = SETTINGS_GAMMA8_BLUE_DEFAULT;
	}
	
	
	for(i=0;i<VIRTUAL_SLA_NUMBER;i++)
	{
		uiSettingsVirtualSLALength[i] = 0;
		uiSettingsVirtualSLAState[i] = SLA_DEACTIVE;
		uiSettingsVirtualSLAModeActive[i] = 0;
		
		for(j=0;j<MODE_PARAMETER_NUMBER;j++)
		{
			uiSettingsVirtualSLAModeParameter[i][j] = 0;
		}
		
		for(j=0;j<VIRTUAL_SLA_SEGMENTS_NUMBER;j++)
		{
			tsSettingsVirtualSLAMap[i][j].uiDestSLA=VIRTUAL_SLA_DEST_NONE;
			tsSettingsVirtualSLAMap[i][j].bInverted=FALSE;
			tsSettingsVirtualSLAMap[i][j].uiSegmentLength=0;
			tsSettingsVirtualSLAMap[i][j].uiSourceLEDStart=0;
			tsSettingsVirtualSLAMap[i][j].uiSourceLEDEnd=0;
		}
	}
}