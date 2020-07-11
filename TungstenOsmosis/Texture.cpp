#include "pch.h"
#include "Texture.h"


TungstenOsmosis::Texture::Texture()
{
}

TungstenOsmosis::Texture::Texture(Tungsten::Texture * arg)
{
	_impl = arg;
}

TungstenOsmosis::Texture::Texture(Tungsten::Texture arg)
{
	_impl = new Tungsten::Texture();
	(*_impl) = arg;
}
