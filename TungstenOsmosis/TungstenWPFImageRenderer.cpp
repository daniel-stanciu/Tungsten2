#include "pch.h"
#include "TungstenWPFImageRenderer.h"

using namespace System::Windows::Media;
using namespace System::Windows;
using namespace System;

Tungsten::TungstenWPFImageRenderer::TungstenWPFImageRenderer(Image ^ image)
{

	UINT flags = 0;
	flags |= D3D11_CREATE_DEVICE_SINGLETHREADED; //D3D calls will only come from one thread
	flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//required for D2D interop

	flags |= D3D11_CREATE_DEVICE_DEBUG;


	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&_device,
		nullptr,
		&_context
	);
	


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	texDesc.Height = (UINT)image->Height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Width = (UINT)image->Width;

	auto writeable = gcnew WriteableBitmap((int)image->Width, (int)image->Height, 96, 96, PixelFormats::Bgra32,nullptr);
	image->Source = writeable;




	_device->CreateTexture2D(&texDesc, NULL, &_backBuffer);
	_device->CreateRenderTargetView(_backBuffer, NULL, &_renderTarget);


	texDesc.BindFlags = 0;
	texDesc.Usage = D3D11_USAGE_STAGING;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	_device->CreateTexture2D(&texDesc, NULL, &_staging);

	ID3D11Texture2D* depthTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = NULL;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.Height = (UINT)image->Height;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.MiscFlags = 0;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Width = (UINT)image->Width;


	_device->CreateTexture2D(&depthTextureDesc, NULL, &depthTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	_device->CreateDepthStencilView(depthTexture, &descDSV, &_depthStencil);
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.StencilEnable = false;

	_device->CreateDepthStencilState(&depthStateDesc, &_depthStencilState);

	_context->OMSetRenderTargets(1, &_renderTarget, _depthStencil);
	_context->OMSetDepthStencilState(_depthStencilState, 0);

	CD3D11_VIEWPORT vp
	{
		0.0f,
		0.0f,
		(float)image->Width,
		(float)image->Height
	};

	_context->RSSetViewports(1, &vp);


}

ID3D11Device * Tungsten::TungstenWPFImageRenderer::Device()
{
	return _device;
}

ID3D11DeviceContext * Tungsten::TungstenWPFImageRenderer::Context()
{
	return _context;
}

void Tungsten::TungstenWPFImageRenderer::Present()
{
	
}

void Tungsten::TungstenWPFImageRenderer::Clear(float4 color)
{
	_context->ClearDepthStencilView(_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, (UINT8)0.0f);
	float clearColor[4] = { color.x,color.y,color.z,color.w };
	_context->ClearRenderTargetView(_renderTarget, clearColor);
}

void Tungsten::TungstenWPFImageRenderer::ResetRenderTarget()
{
	_context->OMSetRenderTargets(1, &_renderTarget, _depthStencil);
	_context->OMSetDepthStencilState(_depthStencilState, 0);
}

void Tungsten::TungstenWPFImageRenderer::Project(Image ^ target)
{



	_context->CopyResource(_staging, _backBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedBackBuffer;
	_context->Map(_staging, 0, D3D11_MAP_READ, 0, &mappedBackBuffer);

	auto writeableBitMap = (WriteableBitmap^)target->Source;
	Int32Rect rect = Int32Rect(0, 0, 500,500 );

	System::IntPtr dataptr((long long)reinterpret_cast<unsigned long long>(mappedBackBuffer.pData));
	auto dataLength = rect.Width*rect.Height;
	
	try 
	{
		
		writeableBitMap->WritePixels(rect, dataptr, mappedBackBuffer.RowPitch*500 ,mappedBackBuffer.RowPitch);
	
	}
	catch (Exception^ ex)
	{
		auto ex2 = ex->InnerException;
	}

	_context->Unmap(_staging, 0);
}


