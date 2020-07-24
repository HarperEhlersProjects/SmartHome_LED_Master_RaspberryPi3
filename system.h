/*-------------------Description of settings module-----------------------
The settings module contains every setting that can be changed by the user. Watch below for detailed descriptions.
*/
#ifndef SYSTEM_H
#define SYSTEM_H

#include "makros.h"
#include "graphics.h"
#include "game_box.h"

/*
uiDestSLA: unsigned integer that defines the physical SLA on which the pixel segment will be mapped.
bSLAInverted : boolean value for inverse mapping.
length : unsigned integer for the length of virtual SLA.
firstSourceLED : unsigned integer that defines the led number on the virtual SLA where the segment has to start.
firstDestLED : unsigned integer that defines the led number on the physical SLA where the segment has to start.
*/
typedef struct {
	int uiDestSLA;
	char bSLAInverted;

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
	char bIsActive;
	char bEnabled;
	char uiPriority;
	double uiParameter[EVENT_HANDLER_PARAMETER_NUMBER];
	double uiModeParameter[MODE_PARAMETER_NUMBER];
}tsEventHandler;


typedef struct {
	char Number;								//Active mode
	double Parameter[MODE_PARAMETER_NUMBER];	//Parameters of activated mode
	int Actors[MODE_ACTOR_NUMBER];
}tsMode;

class SLA
{
public:
	char state;		//state of physical SLA: active or inactive
	char colorType;	//type of physical SLA. e.g. RGB or GBR
	long length;	//length of physical SLAs

	SLA()
	{
		state = SLA_DEACTIVE;
		colorType = SETTINGS_SLATYPE_RGB;
		length = SLA_LENGTH_MAX;
	}
};

class VirtualSLA
{
public:
	long length;	//length of virtual SLA

	tsMode mode;

	tsSegment segments[VIRTUAL_SLA_SEGMENTS_NUMBER];

	GraphicsTB graphics;

	DPU display;

	VirtualSLA()
	{
		char i;

		length = 0;
		mode.Number = MODES_MODE_BLANK;

		for(i = i; i < MODE_PARAMETER_NUMBER; i++)
		{
			mode.Parameter[i] = 0;
		}

		for (i = i; i < MODE_ACTOR_NUMBER; i++)
		{
			mode.Actors[i] = 0;
		}

		for (i=0;i<VIRTUAL_SLA_SEGMENTS_NUMBER;i++)
		{
			segments[i] = { VIRTUAL_SLA_DEST_NONE,FALSE,0,0,0};
		}

		graphics = GraphicsTB();
	}

	void serializeDPUMatrix();

};

class System
{
public:
	long SLALengthMax;
	VirtualSLA virtualSLAs[VIRTUAL_SLA_NUMBER];
	SLA SLAs[SLA_NUMBER];

	GameBox gamebox;
	System()
	{
		char i;

		SLALengthMax = SLA_LENGTH_MAX;

		for (i = 0; i < SLA_NUMBER; i++)
		{
			virtualSLAs[i] = VirtualSLA();
		}

		for (i = 0; i < SLA_NUMBER; i++)
		{
			SLAs[i] = SLA();
		}

		gamebox = GameBox();
	}
};

#endif /* SYSTEM_H */
