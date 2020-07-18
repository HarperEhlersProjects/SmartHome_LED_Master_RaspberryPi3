#ifndef SETTINGS_H
#define SETTINGS_H

#include "system.h"
#include "rapidjson.h"

class Settings
{
public:
	System* system;
	//dom::parser parser;
	//dom::element config_file;
	Settings(System* system);

	void downSync();
	void upSync();
};



#endif