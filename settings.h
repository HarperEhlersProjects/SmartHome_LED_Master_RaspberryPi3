/*-------------------Description of settings module-----------------------
The settings module contains every setting that can be changed by the user. Watch below for detailed descriptions.
*/
#ifndef SETTINGS_H
#define SETTINGS_H

#include "makros.h"

/*
Structure with metadata for the mapping of a pixel segment.

uiDestSLA: unsigned integer that defines the physical SLA on which the pixel segment will be mapped.
bSLAInverted: boolean value for inverse mapping.
uiSegmentLength: unsigned integer for the length of virtual SLA.
uiSourceLEDStart: unsigned integer that defines the led number on the virtual SLA where the segment has to start.
uiDesstLEDStart: unsigned integer that defines the led number on the physical SLA where the segment has to start.
*/
typedef struct{
	int uiDestSLA;
	char bSLAInverted;
	
	long uiSegmentLength;
	long uiSourceLEDStart;
	long uiDestLEDStart;
}tsVirtualSLAMap;

/*
Structure that defines all settings of an event handler.
bIsActive: boolean value which shows if the event handler has been triggered. 
bEnabled: boolean value which defines if the event handler is triggerable.
uiPriority: unsigned integer which defines the priority of event handler. Higher number cause a higher priorisation in case of trigger collisions.
uiParameter: array of unsiged integer which defines the behavior of associated event handler.
uiModeParameter: array of unsigned integer which defines the parameter values of a mode that has been triggered by the event handler.
*/
typedef struct{
	char bIsActive;
	char bEnabled;
	char uiPriority;
	double uiParameter[EVENT_HANDLER_PARAMETER_NUMBER];
	double uiModeParameter[MODE_PARAMETER_NUMBER];
}tsEventHandler;


extern long uiSettingsVirtualSLALength[VIRTUAL_SLA_NUMBER];//length for the virtual SLAs
extern char uiSettingsModeActive[VIRTUAL_SLA_NUMBER];//Active Mode for every virtual SLA
extern double uiSettingsModeParameter[VIRTUAL_SLA_NUMBER][MODE_PARAMETER_NUMBER];//Parameters of activated mode for every virtual SLA
extern tsVirtualSLAMap tsSettingsVirtualSLAMap[VIRTUAL_SLA_NUMBER][VIRTUAL_SLA_SEGMENTS_NUMBER];//Every virtual SLA owns a collection of segment mapping structures

extern long uiSettingsSLALengthMAX;//maximum length of physical SLA

extern char uiSettingsSLAType[SLA_NUMBER];//type of physical SLA. e.g. RGB or GBR
extern long uiSettingsSLALength[SLA_NUMBER];//length of physical SLAs
extern char uiSettingsSLAState[SLA_NUMBER];//state of physical SLA: active or inactive

extern float fSettingsGamma8RedValue[VIRTUAL_SLA_NUMBER];//gamma value of red for every SLA
extern float fSettingsGamma8GreenValue[VIRTUAL_SLA_NUMBER];//gamma values of green for every SLA
extern float fSettingsGamma8BlueValue[VIRTUAL_SLA_NUMBER];//gamma values of blue for every SLA

extern tsEventHandler tsSettingsEventHandler[EVENT_HANDLER_NUMBER];//Structures of event handler configurations 

void vSettingsInit(void);


//class VirtualSLA
//{
//
//public:
//
//
//};



#endif /* SETTING_H */
