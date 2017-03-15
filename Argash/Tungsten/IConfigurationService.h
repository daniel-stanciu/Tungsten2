#pragma once
#include "pch.h"
namespace Tungsten
{
struct ConfigurationState
{
	enum windowedState
	{
		yes,
		no,
		borderless
	} windowed;
	struct
	{
		int height;
		int width;
	} resolution;

};

class IConfigurationService
{
public:
	virtual void				Load(WCHAR*)=0;
	virtual void				Set(ConfigurationState)=0;
	virtual ConfigurationState	Get()=0;
	virtual void				Save()=0;

};

}