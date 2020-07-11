#pragma once

#include "IConfigurationService.h"
namespace Tungsten {
	class ConfigManager : public IConfigurationService
	{
	public:

		virtual void Load(const char* path);
		virtual Tungsten::ConfigurationState Get();
		virtual void Set(Tungsten::ConfigurationState state);
		virtual void Save();
		ConfigManager(void);
		~ConfigManager(void);
	private:
		Tungsten::ConfigurationState currentState;

	};

}