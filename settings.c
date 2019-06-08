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
	}
}


void vSettingsSetSLALength(char uiSLANumber,char uiLength)
{
	uiSettingsSLALength[uiSLANumber] = uiLength;
}

char vSettingsGetSLALength(char uiSLANumber)
{
	return uiSettingsSLALength[uiSLANumber];
}


void vSettingsSetSLAState(char uiSLANumber,char uiState)
{
	uiSettingsSLALength[uiSLANumber] = uiState;
}

char vSettingsGetSLAState(char uiSLANumber)
{
	return uiSettingsSLALength[uiSLANumber];
}


void vSettingsSetModeActive(char uiSLANumber,char uiMode)
{
	uiSettingsSLALength[uiSLANumber] = uiMode;
}

char vSettingsGetModeActive(char uiSLANumber)
{
	return uiSettingsModeActive[uiSLANumber];
}


void vSettingsSetModeParameter(char uiSLANumber,char uiParameter, char uiValue)
{
	uiSettingsModeParameter[uiSLANumber][uiParameter] = uiValue;
}

char vSettingsGetModeParameter(char uiSLANumber, char uiParameter)
{
	return uiSettingsModeParameter[uiSLANumber][uiParameter];
}
