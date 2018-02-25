#pragma once
namespace Tungsten {
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
}
