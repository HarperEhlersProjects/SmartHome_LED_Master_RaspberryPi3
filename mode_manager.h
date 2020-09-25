#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include "makros.h"
#include "system.h"


class ModeManager
{
public:
	System* system;

	ModeManager(System* system): system(system){}

	void vFrameCalculate(void);

	//Methods for defining mode behavior

	void vMode0(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB: 0-255
	*/
	void vMode1(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB: 0-255
	parameter 3: snake length: 0-255
	parameter 4: snake veloity: 0-255*/
	void vMode2(VirtualSLA* vSLA);

	/*
	parameter 0-2: RGB_influence: 0-255
	parameter 3: ShrinkingVelocity: 0-255
	parameter 4: expanding velocity: 0-255*/
	void vMode3(VirtualSLA* vSLA);

	//Color Fade
	void vMode4(VirtualSLA* vSLA);

	//Game Box
	void vMode5(VirtualSLA* vSLA);

};

#endif /* MODE_MANAGER_H */