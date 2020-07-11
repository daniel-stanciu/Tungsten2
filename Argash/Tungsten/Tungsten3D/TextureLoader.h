#pragma once
#include <map>
#include "Texture.h"
namespace Tungsten {
	class TextureLoader
	{
	private:
		static std::map<std::wstring, Texture> textureCache;
	public:
		static Texture Get(std::wstring);
	};

}