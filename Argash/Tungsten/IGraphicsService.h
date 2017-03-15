#pragma once
#include "pch.h"
#include "DataContainers.h"
#include "Sprite.h"
namespace Tungsten
{
	
struct Texture
{
	ID3D11ShaderResourceView*	shaderView;
	ID3D11Resource*				resource;

};
struct SkyMap
{
	ID3D11Texture2D* textures[6];
};
enum GraphicsState
{
	SpriteRender,
	SimpleModel,
	SkyBox,
	Grass
};
class IGraphicsService
{
public:
	virtual Texture LoadTexture(WCHAR* )=0;	
	virtual Texture	LoadSkyMap()=0;
	virtual void	DrawInstanced(MemoryBlock vertexData,MemoryBlock indexData,MemoryBlock instanceData)=0;
	virtual void	Refresh()=0;
	virtual void	SetBuffers(MemoryBlock vertexData,MemoryBlock indexData, MemoryBlock constantData=MemoryBlock::Null())=0;
	virtual void	DrawIndexed(UINT IndexCount,UINT StartIndexLocation,INT BaseVertexLocation)=0;
	virtual void	SetTexture(Texture)=0;
	virtual void	Present()=0;
	virtual void	SetState(GraphicsState)=0;
	virtual void	SetConstantBufferVS(MemoryBlock,int)=0;
	virtual void	SetConstantBufferPS(MemoryBlock,int)=0;
};


}