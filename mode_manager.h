#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include "makros.h"
#include "system.h"

class ModeManager
{
public:
	VirtualSLA* virtualSLAs;

	//Constructor
	ModeManager(VirtualSLA* virtualSLAs)
	{
		char i, j;

		this->virtualSLAs = virtualSLAs;
	}

	void vFrameCalculate(void);


	//Methods for defining mode behavior

	static void vMode0(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB: 0-255
	parameter 3: GammaorrectionEnable: boolean*/
	static void vMode1(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB: 0-255
	parameter 3: GammaorrectionEnable: boolean
	parameter 4: snake length: 0-255
	parameter 5: snake veloity: 0-255*/
	static void vMode2(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB_influence: 0-255
	parameter 3: GammaorrectionEnable: boolean
	parameter 4: ShrinkingVelocity: 0-255
	parameter 5: expanding velocity: 0-255*/
	static void vMode3(VirtualSLA* vSLA);

	//Color Fade
	static void vMode4(VirtualSLA* vSLA);

	//Game Box
	static void vMode5(VirtualSLA* vSLA);

};

#endif /* MODE_MANAGER_H */