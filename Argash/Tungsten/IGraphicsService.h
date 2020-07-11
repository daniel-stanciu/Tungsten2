#pragma once


#include <d3d11_1.h>
#include "TungstenMath/Vector.h"
namespace Tungsten
{
//	
//
//struct SkyMap
//{
//	ID3D11Texture2D* textures[6];
//};

class IGraphicsService
{
public:
	//virtual Texture LoadTexture(const WCHAR* )=0;	
	//virtual Texture	LoadSkyMap()=0;
	//virtual void	DrawInstanced(MemoryBlock vertexData,MemoryBlock indexData,MemoryBlock instanceData)=0;
	//virtual void	Refresh()=0;
	//virtual void	SetBuffers(MemoryBlock vertexData,MemoryBlock indexData, MemoryBlock constantData=MemoryBlock::Null())=0;
	//virtual void	DrawIndexed(UINT IndexCount,UINT StartIndexLocation,INT BaseVertexLocation)=0;
	////virtual void	SetTexture(Texture)=0;
	//virtual void	Present()=0;
	//virtual void	SetState(GraphicsState)=0;
	//virtual void	SetConstantBufferVS(MemoryBlock,int)=0;
	//virtual void	SetConstantBufferPS(MemoryBlock,int)=0;
	virtual ID3D11Device* Device() = 0;
	virtual ID3D11DeviceContext* Context() = 0;
	virtual void Present() = 0;
	virtual void Clear(float4 color = { 0.0f,0.0f,0.0f,1.0f }) = 0;
	virtual void ResetRenderTarget() = 0;
};


}