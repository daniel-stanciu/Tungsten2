#include "pch.h"
#include "Sprite.h"
using namespace DirectX;

Sprite::Sprite()
{

}

Sprite::Sprite(XMFLOAT2 topLeft,XMFLOAT2 bottomRight,float depth,XMFLOAT4 tint)
{
	//Normalise in the 0-1 interval
	depth=depth/100;
	/*
	0---1
	| @ |
	2---3
	*/
	vertices[0].pos=(XMFLOAT4(XTr(topLeft.x),YTr(topLeft.y)			,depth,1.0f));
	vertices[1].pos=(XMFLOAT4(XTr(bottomRight.x),YTr(topLeft.y)		,depth,1.0f));
	vertices[2].pos=(XMFLOAT4(XTr(topLeft.x),YTr(bottomRight.y)		,depth,1.0f));
	vertices[3].pos=(XMFLOAT4(XTr(bottomRight.x),YTr(bottomRight.y) ,depth,1.0f));

	vertices[0].tint=tint;
	vertices[1].tint=tint;
	vertices[2].tint=tint;
	vertices[3].tint=tint;
	
	
}

Sprite::~Sprite()
{

}

void Sprite::SetTextureCoords(XMFLOAT2 topLeft,XMFLOAT2 bottomRight)
{
	vertices[0].tex=XMFLOAT2(topLeft.x,topLeft.y);
	vertices[1].tex=XMFLOAT2(bottomRight.x,topLeft.y);
	vertices[2].tex=XMFLOAT2(topLeft.x,bottomRight.y);
	vertices[3].tex=XMFLOAT2(bottomRight.x,bottomRight.y);
	
}

float Sprite::GetHeight()
{
	return (vertices[0].pos.y-vertices[2].pos.y)/2;
}

float Sprite::GetWidth()
{
	return (vertices[1].pos.x-vertices[0].pos.x)/2;
}

float Sprite::XTr(float x)
{
	return x*2-1;
}

float Sprite::YTr(float y)
{
	return -(y*2-1);
}

float Sprite::iXTr(float x)
{
	return (x+1)/2;
}

float Sprite::iYTr(float y)
{
	return -((y+1)/2);
}