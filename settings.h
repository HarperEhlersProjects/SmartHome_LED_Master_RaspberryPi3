#ifndef SETTINGS_H
#define SETTINGS_H


#include "makros.h"

long uiSettingsSLALengthMAX;

long uiSettingsSLALength[SLA_NUMBER];
char uiSettingsSLAState[SLA_NUMBER];
char uiSettingsModeActive[SLA_NUMBER];
double uiSettingsModeParameter[SLA_NUMBER][MODE_PARAMETER_NUMBER];

float fSettingsGamma8RedValue[SLA_NUMBER];
float fSettingsGamma8GreenValue[SLA_NUMBER];
float fSettingsGamma8BlueValue[SLA_NUMBER];


void vSettingsInit(void);

void vSettingsSetSLALength(char uiSLANumber,char uiLength);
char vSettingsGetSLALength(char uiSLANumber);

void vSettingsSetSLAState(char uiSLANumber,char uiState);
char vSettingsGetSLAState(char uiSLANumber);

void vSettingsSetModeActive(char uiSLANumber,char uiMode);
char vSettingsGetModeActive(char uiSLANumber);

void vSettingsSetModeParameter(char uiSLANumber,char uiParameter, double uiValue);
double vSettingsGetModeParameter(char uiSLANumber, char uiParameter);

#endif /* SETTING_H */
