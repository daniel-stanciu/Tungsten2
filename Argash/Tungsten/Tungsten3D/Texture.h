#pragma once
#include <d3d11_1.h>
namespace Tungsten
{
	class Texture
	{
	public:
		ID3D11ShaderResourceView * shaderView;
		ID3D11Resource*				resource;
	};
}