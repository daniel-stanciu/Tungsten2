#pragma once
#include "Tungsten\Tungsten3D.h"
#include "Texture.h"

namespace TungstenOsmosis
{
	public ref class TextureLoader
	{
	internal:
		Tungsten::TextureLoader* _impl;
	public:
		TextureLoader();
		Texture^ Get(System::String^ arg);
	};
}


