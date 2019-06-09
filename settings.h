#ifndef SETTINGS_H
#define SETTINGS_H


#include "makros.h"

long uiSettingsSLALengthMAX;

char uiSettingsSLALength[SLA_NUMBER];
char uiSettingsSLAState[SLA_NUMBER];
char uiSettingsModeActive[SLA_NUMBER];
char uiSettingsModeParameter[SLA_NUMBER][MODE_PARAMETER_NUMBER];

void vSettingsInit(void);

void vSettingsSetSLALength(char uiSLANumber,char uiLength);
char vSettingsGetSLALength(char uiSLANumber);

void vSettingsSetSLAState(char uiSLANumber,char uiState);
char vSettingsGetSLAState(char uiSLANumber);

void vSettingsSetModeActive(char uiSLANumber,char uiMode);
char vSettingsGetModeActive(char uiSLANumber);

void vSettingsSetModeParameter(char uiSLANumber,char uiParameter, char uiValue);
char vSettingsGetModeParameter(char uiSLANumber, char uiParameter);

#endif /* SETTING_H */
