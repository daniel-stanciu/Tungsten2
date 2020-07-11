#pragma once
namespace Tungsten {
	struct ConfigurationState
	{
		/*enum windowedState
		{
			yes,
			no,
			borderless
		} windowed;*/
		struct
		{
			int Height;
			int Width;
		} RenderTargetSize;

	};
}
