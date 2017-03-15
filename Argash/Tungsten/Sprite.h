#pragma once
#include <pch.h>
#include "Timer.h"



struct SpriteVertex
{
	//Screen Position
	DirectX::XMFLOAT4 pos;
	//The position to be sampled from the texture
	DirectX::XMFLOAT2 tex;
	//Tint to change colour
	DirectX::XMFLOAT4 tint;
	
};
class Sprite
{
private:
	//All messages to/from this class are defined in ScreenSpace
	//UV Space to screen space transform
	inline float XTr(float x);
	inline float YTr(float y);
	//Inverses
	inline float iXTr(float x);
	inline float iYTr(float y);
public:
	SpriteVertex vertices[4];
	Sprite();
	Sprite(DirectX::XMFLOAT2 topLeft,DirectX::XMFLOAT2 bottomRight,float depth=1,DirectX::XMFLOAT4 tint=DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f));
	void SetTextureCoords	(DirectX::XMFLOAT2 topleft,DirectX::XMFLOAT2 bottomright);
	float GetWidth();
	float GetHeight();
	~Sprite					(void);
	
};

