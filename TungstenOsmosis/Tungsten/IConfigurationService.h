#pragma once

#include "ConfigurationState.h"
namespace Tungsten
{


class IConfigurationService
{
public:
	virtual void				Load(const char*)=0;
	virtual void				Set(ConfigurationState)=0;
	virtual ConfigurationState	Get()=0;
	virtual void				Save()=0;

};

}