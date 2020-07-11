#include "SpriteImmediateEffect.h"
#include "../DataContainers.h"
#include "../PlatformTools.h"
#include "../ServiceLocator.h"
using namespace Tungsten;

SpriteImmediateEffect::SpriteImmediateEffect()
{
	MemoryBlock bytes;
	_device = Services.Graphics()->Device();
	_context = Services.Graphics()->Context();
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSpriteRender.cso", &bytes);
	_device->CreatePixelShader(bytes.ptr, bytes.width, NULL, &_PS);
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSpriteRender.cso", &bytes);
	_device->CreateVertexShader(bytes.ptr, bytes.width, NULL, &_VS);
	D3D11_INPUT_ELEMENT_DESC IL_desc[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },		
		{ "TINT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
		{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL }
	};
	_device->CreateInputLayout(IL_desc, 3, bytes.ptr, bytes.width, &_IL);

	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = NULL;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_device->CreateBuffer(&bufferDesc, nullptr, &_surfaceDimensionsBuffer);
	_device->CreateBuffer(&bufferDesc, nullptr, &_textureDimensionsBuffer);

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(SpriteVertex)*4;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_device->CreateBuffer(&bufferDesc, nullptr, &_vertexBuffer);

	float borderColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	D3D11_SAMPLER_DESC sampler;
	sampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler.ComparisonFunc = D3D11_COMPARISON_GREATER;
	sampler.MaxAnisotropy = 16;
	sampler.MipLODBias = 0;
	sampler.Filter = D3D11_FILTER_ANISOTROPIC;
	_device->CreateSamplerState(&sampler, &_samplerState);

}


SpriteImmediateEffect::~SpriteImmediateEffect()
{
}

void SpriteImmediateEffect::Apply()
{
	_context->PSSetShader(_PS, nullptr,0);
	_context->VSSetShader(_VS, nullptr, 0);
	_context->IASetInputLayout(_IL);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT x = 0;
	UINT stride = sizeof(SpriteVertex);
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride,&x);
	_context->PSSetConstantBuffers(0, 1, &_textureDimensionsBuffer);
	_context->VSSetConstantBuffers(0, 1, &_surfaceDimensionsBuffer);

	_context->PSSetSamplers(0,1, &_samplerState);
}

void SpriteImmediateEffect::Draw()
{
	/*ID3D11RenderTargetView* rt;
	_context->OMGetRenderTargets(1, &rt, nullptr);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	rt->GetDesc(&rtv_desc);*/
	_context->Draw(4, 0);

}

void Tungsten::SpriteImmediateEffect::SetSprrite(Sprite arg)
{
	auto points = arg.Position.ClockWise();
	for (int i = 0; i < 4; i++)
	{
		_vertices[i].pos.x = points[i].x;
		_vertices[i].pos.y = points[i].y;
		_vertices[i].pos.z = 0.5f;
		_vertices[i].pos.w = 1.0f;
	}
	points = arg.TextureSample.ClockWise();
	for (int i = 0; i < 4; i++)
	{
		_vertices[i].tex.x = points[i].x;
		_vertices[i].tex.y = points[i].y;	
		_vertices[i].tint = { 1.0f,1.0f,1.0f,1.0f };
	}

	ID3D11Texture2D* texture;
	arg.Texture.resource->QueryInterface(IID_PPV_ARGS(&texture));
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetDesc(&texDesc);
	_textureDimensions.x = (float)texDesc.Width;
	_textureDimensions.y = (float)texDesc.Height;

	//_context->UpdateSubresource(_textureDimensionsBuffer, 0, nullptr, &_textureDimensions, 0, 0);
	D3D11_MAPPED_SUBRESOURCE mappedResourceTexture;
	_context->Map(_textureDimensionsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceTexture);
	memcpy(mappedResourceTexture.pData, &_textureDimensions, sizeof(_textureDimensions));
	_context->Unmap(_textureDimensionsBuffer, 0);
	texture->Release();

	_context->PSSetShaderResources(0, 1, &arg.Texture.shaderView);

	ID3D11RenderTargetView* rtv;
	_context->OMGetRenderTargets(1, &rtv, nullptr);
	ID3D11Resource* rtv_res;
	rtv->GetResource(&rtv_res);
	rtv_res->QueryInterface(IID_PPV_ARGS(&texture));


	D3D11_MAPPED_SUBRESOURCE mappedResourceSurface;
	texture->GetDesc(&texDesc);
	_surfaceDimensions.x = (float)texDesc.Width;
	_surfaceDimensions.y = (float)texDesc.Height;
	_context->Map(_surfaceDimensionsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceSurface);
	memcpy(mappedResourceSurface.pData, &_surfaceDimensions, sizeof(_surfaceDimensions));
	_context->Unmap(_surfaceDimensionsBuffer, 0);
	rtv->Release();
	rtv_res->Release();
	texture->Release();


	D3D11_MAPPED_SUBRESOURCE mappedResourceVertex;
	_context->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVertex);
	memcpy(mappedResourceVertex.pData, &_vertices, 4 * sizeof(SpriteVertex));
	_context->Unmap(_vertexBuffer, 0);


}

void Tungsten::SpriteImmediateEffect::DrawSprite(Sprite arg)
{
	
	SetSprrite(arg);
	Apply();
	Draw();
}
