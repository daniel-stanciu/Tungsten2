
#include "ConfigManager.h"
#include "PlatformTools.h"
using namespace std;
using namespace Tungsten;
ConfigManager::ConfigManager(void)
{
}


ConfigManager::~ConfigManager(void)
{
}

void ConfigManager::Load(const char* path)
{
	stringstream* fileStream;
	//Read file
	Tungsten::PlatformTools::CreateStringBufferFromFile((WCHAR*)path,&fileStream);
	string line;
	
	
	while(!fileStream->eof())
	{
		getline(*fileStream,line);
		int splitPosition=line.find("=");
		string tag=line.substr(0,splitPosition);
		string value=line.substr(splitPosition+1,line.size()-splitPosition-2);
		//TODO:Hardcode tags and values

		if(tag=="windowed")
		{
			if(value=="yes") currentState.windowed=currentState.yes;
			if(value=="no") currentState.windowed=currentState.no;
			if(value=="borderless") currentState.windowed=currentState.borderless;
		}
		if(tag=="width")
		{
			currentState.resolution.width=atoi(value.c_str());
		}
		if(tag=="height")
		{
			currentState.resolution.height=atoi(value.c_str());
		}
	}
}

ConfigurationState ConfigManager::Get()
{
	return currentState;
}

void ConfigManager::Set(ConfigurationState state)
{
	currentState=state;
}

void ConfigManager::Save()
{
	//  TODO: IMPLEMENTATION
}