#ifndef MODES_H
#define MODES_H

#include "makros.h"

void vModesInit(void);

void vModesFrameCalculate(void);

void vMode0(char uiSLA);
void vMode1(char uiSLA);
void vMode2(char uiSLA);
void vMode3(char uiSLA);
void vMode4(char uiSLA);
void vMode5(char uiSLA);

class ModeManager
{
private:
	int piModeActors[VIRTUAL_SLA_NUMBER][MODE_ACTOR_NUMBER];

public:

	//Constructor
	ModeManager()
	{
		char i, j;

		for (i = 0; i < VIRTUAL_SLA_NUMBER; i++)
		{
			for (j = 0; j < MODE_ACTOR_NUMBER; j++)
			{
				piModeActors[i][j] = 0;
			}
		}
	}

	void vFrameCalculate(void);


	//Methods for defining mode behavior

	void vMode0(char uiSLA);

	/*
	parameter 0-2: RGB: 0-255
	parameter 3: GammaorrectionEnable: boolean*/
	void vMode1(char uiSLA);

	/*
	parameter 0-2: RGB: 0-255
	parameter 3: GammaorrectionEnable: boolean
	parameter 4: snake length: 0-255
	parameter 5: snake veloity: 0-255*/
	void vMode2(char uiSLA);

	/*
	parameter 0-2: RGB_influence: 0-255
	parameter 3: GammaorrectionEnable: boolean
	parameter 4: ShrinkingVelocity: 0-255
	parameter 5: expanding velocity: 0-255*/
	void vMode3(char uiSLA);

	//Color Fade
	void vMode4(char uiSLA);

	//Game Box
	void vMode5(char uiSLA);

};

#endif /* MODES_H */