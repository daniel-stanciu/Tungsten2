#pragma once
#include "pch.h"
#include "Tungsten\IConfigurationService.h"
class ConfigManager: public Tungsten::IConfigurationService
{
public:
	
	virtual void Load(WCHAR* path);
	virtual Tungsten::ConfigurationState Get();
	virtual void Set(Tungsten::ConfigurationState state);
	virtual void Save();
	ConfigManager(void);
	~ConfigManager(void);
private:
	Tungsten::ConfigurationState currentState;
	
};

