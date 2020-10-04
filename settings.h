#ifndef SETTINGS_H
#define SETTINGS_H


#include "makros.h"
#include "system.h"

class Settings
{
public:
	System* system;
	
	Settings(System* system);

	void downSync();
	void Sync();
};



#endif