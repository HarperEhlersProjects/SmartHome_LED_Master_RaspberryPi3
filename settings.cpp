#include "settings.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/istreamwrapper.h"

using namespace rapidjson;
using namespace std;

Settings::Settings(System* system)
{
	this->system = system;

	downSync();
	upSync();
}


void Settings::downSync()
{
	char i, j;

	Document config_json;
	config_json.SetObject();
	Document::AllocatorType& allocator = config_json.GetAllocator();
	StringBuffer strbuf;
	PrettyWriter<rapidjson::StringBuffer> writer(strbuf);

	Value virtualSLAs(kArrayType);
	Value modeParameter(kArrayType);
	Value segments(kArrayType);
	Value val(kObjectType);
	Value obj(kObjectType);
	Value obj2(kObjectType);
	Value SLAs(kArrayType);

	config_json.AddMember("SLALengthMax", val.SetInt(system->SLALengthMax), allocator);

	for (i = 0; i < VIRTUAL_SLA_NUMBER; i++)
	{
		modeParameter.SetArray();
		for (j = 0; j < MODE_PARAMETER_NUMBER; j++)
		{
			modeParameter.PushBack(val.SetDouble(system->virtualSLAs[i].mode.Parameter[j]), allocator);
		}

		segments.SetArray();
		for (j = 0; j < VIRTUAL_SLA_SEGMENTS_NUMBER; j++)
		{
			obj2.SetObject();

			obj2.AddMember("inverted", val.SetBool(system->virtualSLAs[i].segments[j].bSLAInverted), allocator);
			obj2.AddMember("destSLA", val.SetInt(system->virtualSLAs[i].segments[j].uiDestSLA), allocator);
			obj2.AddMember("length", val.SetInt(system->virtualSLAs[i].segments[j].length), allocator);
			obj2.AddMember("firstSourceLED", val.SetInt(system->virtualSLAs[i].segments[j].firstSourceLED), allocator);
			obj2.AddMember("firstDestLED", val.SetInt(system->virtualSLAs[i].segments[j].firstDestLED), allocator);
			segments.PushBack(obj2, allocator);
		}

		obj.SetObject();
		obj.AddMember("length", val.SetInt(system->virtualSLAs[i].length), allocator);
		obj.AddMember("mode", val.SetInt(system->virtualSLAs[i].mode.Number), allocator);
		obj.AddMember("modeParameter", modeParameter, allocator);
		obj.AddMember("segments", segments, allocator);

		virtualSLAs.PushBack(obj, allocator);
	}
	config_json.AddMember("virtualSLAs", virtualSLAs, allocator);

	for (i = 0; i < SLA_NUMBER; i++)
	{
		obj.SetObject();
		obj.AddMember("state", val.SetInt(system->SLAs[i].state), allocator);
		obj.AddMember("length", val.SetInt(system->SLAs[i].length), allocator);
		obj.AddMember("colorType", val.SetInt(system->SLAs[i].colorType), allocator);
		SLAs.PushBack(obj, allocator);
	}
	config_json.AddMember("SLAs", SLAs, allocator);

	config_json.Accept(writer);

	ofstream config_file(SETTINGS_CONFIG_FILENAME, ios::binary | ios::trunc);
	config_file << strbuf.GetString() << std::endl;
	config_file.close();
}

void Settings::upSync()
{
	char i, j;

	ifstream config_file(SETTINGS_CONFIG_FILENAME);
	if (config_file.is_open())
	{
		IStreamWrapper isw(config_file);
		Document config_json;
		config_json.ParseStream(isw);

		if (config_json.IsObject())
		{

			system->SLALengthMax = config_json["SLALengthMax"].GetInt();


			for (i = 0; i < VIRTUAL_SLA_NUMBER; i++)
			{

				for (j = 0; j < MODE_PARAMETER_NUMBER; j++)
				{
					system->virtualSLAs[i].mode.Parameter[j] = config_json["virtualSLAs"][i]["modeParameter"][j].GetDouble();
				}
				system->virtualSLAs[i].mode.Number = config_json["virtualSLAs"][i]["mode"].GetInt();

				for (j = 0; j < VIRTUAL_SLA_SEGMENTS_NUMBER; j++)
				{
					system->virtualSLAs[i].segments[j].bSLAInverted = config_json["virtualSLAs"][i]["segments"][j]["inverted"].GetBool();
					system->virtualSLAs[i].segments[j].uiDestSLA = config_json["virtualSLAs"][i]["segments"][j]["destSLA"].GetInt();
					system->virtualSLAs[i].segments[j].length = config_json["virtualSLAs"][i]["segments"][j]["length"].GetInt();
					system->virtualSLAs[i].segments[j].firstSourceLED = config_json["virtualSLAs"][i]["segments"][j]["firstSourceLED"].GetInt();
					system->virtualSLAs[i].segments[j].firstDestLED = config_json["virtualSLAs"][i]["segments"][j]["firstDestLED"].GetInt();
				}

				system->virtualSLAs[i].length = config_json["virtualSLAs"][i]["length"].GetInt();
			}


			for (i = 0; i < SLA_NUMBER; i++)
			{
				system->SLAs[i].state = config_json["SLAs"][i]["state"].GetInt();
				system->SLAs[i].length = config_json["SLAs"][i]["length"].GetInt();
				system->SLAs[i].colorType = config_json["SLAs"][i]["colorType"].GetInt();
			}
			config_file.close();
		}
	}
}