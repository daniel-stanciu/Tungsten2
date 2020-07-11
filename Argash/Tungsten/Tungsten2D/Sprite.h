#pragma once
#include "PixelCoordinates.h"
#include "../TungstenMath.h"
#include "../Tungsten3D.h"
//
//
//struct SpriteVertex
//{
//	//Screen Position
//	DirectX::XMFLOAT4 pos;
//	//The position to be sampled from the texture
//	DirectX::XMFLOAT2 tex;
//	//Tint to change colour
//	DirectX::XMFLOAT4 tint;
//	
//};
namespace Tungsten
{
	class Sprite
	{
	public:
		Rectangle<Pixels> Position;
		Rectangle<Pixels> TextureSample;
		Texture Texture;
		Sprite();
	};
}


