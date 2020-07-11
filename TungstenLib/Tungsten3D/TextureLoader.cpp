#include "TextureLoader.h"
#include "../PlatformTools.h"
#include "../ServiceLocator.h"
using namespace Tungsten;
std::map<std::wstring, Texture> TextureLoader::textureCache;
Texture TextureLoader::Get(std::wstring arg)
{
	auto cache = TextureLoader::textureCache;
	auto it = cache.find(arg);
	if (it != cache.end())
		return it->second;
	else
	{
		auto device = Services.Graphics()->Device();
		Texture newTexture;
		if (arg.compare(arg.size() - 3, 3, L".dds")==0)
		{
			DirectX::CreateDDSTextureFromFile(device, arg.c_str(), &newTexture.resource, &newTexture.shaderView);
		}
		else
		{
			auto context = Services.Graphics()->Context();
			DirectX::CreateWICTextureFromFile(device, context, arg.c_str(), &newTexture.resource, &newTexture.shaderView);
		}
		cache.insert({ arg, newTexture });
		return newTexture;
	}
}
