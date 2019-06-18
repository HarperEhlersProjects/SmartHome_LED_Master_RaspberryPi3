#ifndef SETTINGS_H
#define SETTINGS_H


#include "makros.h"

typedef struct{
	int uiDestSLA;
	char bSLAInverted;
	
	long uiSegmentLength;
	long uiSourceLEDStart;
	long uiDestLEDStart;

}tsVirtualSLAMap;

long uiSettingsVirtualSLALength[VIRTUAL_SLA_NUMBER];
char uiSettingsModeActive[VIRTUAL_SLA_NUMBER];
double uiSettingsModeParameter[VIRTUAL_SLA_NUMBER][MODE_PARAMETER_NUMBER];
tsVirtualSLAMap tsSettingsVirtualSLAMap[VIRTUAL_SLA_NUMBER][VIRTUAL_SLA_SEGMENTS_NUMBER];

long uiSettingsSLALengthMAX;

long uiSettingsSLALength[SLA_NUMBER];
char uiSettingsSLAState[SLA_NUMBER];

float fSettingsGamma8RedValue[VIRTUAL_SLA_NUMBER];
float fSettingsGamma8GreenValue[VIRTUAL_SLA_NUMBER];
float fSettingsGamma8BlueValue[VIRTUAL_SLA_NUMBER];


void vSettingsInit(void);


#endif /* SETTING_H */
