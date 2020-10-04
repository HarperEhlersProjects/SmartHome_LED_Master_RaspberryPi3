/*-------------------Description of settings module-----------------------
The settings module contains every setting that can be changed by the user. Watch below for detailed descriptions.
*/
#ifndef SYSTEM_H
#define SYSTEM_H

#pragma once

#include <cstdint>
#include <vector>

#include "makros.h"
#include "framebuffer.h"
#include "games/game_box.h"



/*
uiDestSLA: unsigned integer that defines the physical SLA on which the pixel segment will be mapped.
bSLAInverted : boolean value for inverse mapping.
length : unsigned integer for the length of virtual SLA.
firstSourceLED : unsigned integer that defines the led number on the virtual SLA where the segment has to start.
firstDestLED : unsigned integer that defines the led number on the physical SLA where the segment has to start.
*/
typedef struct {
	int uiDestSLA;
	uint8_t bSLAInverted;

	long length;
	long firstSourceLED;
	long firstDestLED;
}tsSegment;


/*
Structure that defines all settings of an event handler.
bIsActive: boolean value which shows if the event handler has been triggered. 
bEnabled: boolean value which defines if the event handler is triggerable.
uiPriority: unsigned integer which defines the priority of event handler. Higher number cause a higher priorisation in case of trigger collisions.
uiParameter: array of unsiged integer which defines the behavior of associated event handler.
uiModeParameter: array of unsigned integer which defines the parameter values of a mode that has been triggered by the event handler.
*/
typedef struct{
	uint8_t bIsActive;
	uint8_t bEnabled;
	uint8_t uiPriority;
	double uiParameter[EVENT_HANDLER_PARAMETER_NUMBER];
	double uiModeParameter[MODE_PARAMETER_NUMBER];
}tsEventHandler;

/*
Structure that contains all information about the activated mode.
number: integer value which identifies the current mode.
parameter: array of doubles which define the current behavior of the activated mode.
actors: array of doubles which define the current state of the activated mode.
*/
typedef struct {
	uint8_t number;								//Active mode
	double parameter[MODE_PARAMETER_NUMBER];	//Parameters of activated mode
	double actors[MODE_ACTOR_NUMBER];
}tsMode;


/*
The SLA class contains every information about one physical led array.
*/
class SLA
{
public:
	uint8_t state;		//state of physical SLA: active or inactive
	uint8_t colorType;	//type of physical SLA. e.g. RGB or GBR
	long length;	//length of physical SLAs

	SLA(): state(SLA_DEACTIVE),colorType(SETTINGS_SLATYPE_RGB),length(SLA_LENGTH_MAX){}
};

/*
The VirtualSLA class contains the configuration of one virtual led array.
*/
class VirtualSLA
{
public:
	tsMode mode;

	std::vector<tsSegment> segments;

	FrameBuffer framebuffer;	//Containing frame buffer and configuration of gamma correction.

	DPU display;	//Additional display unit which extends the serial framebuffer with an rgb matrix for using the virtual sla as display (i.e. gamebox).

	VirtualSLA();

	void serializeDPUMatrix(void);

	void setFrameBufferSize(long size);
	void setNumberOfSegments(long number);

	long getFrameBufferSize(void);
private:
	long numberOfSegments = 0;
};

/*
The System contains every information about a configuration of virtual LED arrays, physical led arrays and a gamebox.
*/
class System
{
public:
	long SLALengthMax;

	std::vector<VirtualSLA*> virtualSLAs;
	SLA SLAs[SLA_NUMBER];

	GameBox gamebox = GameBox();

	System();
	void setNumberOfvSLAs(long number);

private:
	long numberOfvSLAs = 0;
};

#endif /* SYSTEM_H */
