#pragma once
#include "Tungsten\Tungsten3D.h"
namespace TungstenOsmosis
{
	public ref class Texture
	{
	internal:
		Tungsten::Texture * _impl;
	public:
		Texture();
		Texture(Tungsten::Texture * arg);
		Texture(Tungsten::Texture arg);
	};

}

