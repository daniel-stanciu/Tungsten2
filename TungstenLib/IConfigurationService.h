#pragma once
#include "pch.h"
#include "ConfigurationState.h"
namespace Tungsten
{


class IConfigurationService
{
public:
	virtual void				Load(WCHAR*)=0;
	virtual void				Set(ConfigurationState)=0;
	virtual ConfigurationState	Get()=0;
	virtual void				Save()=0;

};

}