#include "pch.h"
#include "TungstenWin32Renderer.h"


using namespace Tungsten;

TungstenWin32Renderer::TungstenWin32Renderer(HWND hwnd)
{
		UINT flags = 0;
		flags |= D3D11_CREATE_DEVICE_SINGLETHREADED; //D3D calls will only come from one thread
		flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//required for D2D interop
	#if defined _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 640;
		sd.BufferDesc.Height = 480;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&_swapChain,
			&_device,
			nullptr,
			&_context
		);
		ID3D11Texture2D* backbuffer;
		_swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		_device->CreateRenderTargetView(backbuffer, NULL, &_renderTarget);



		ID3D11Texture2D* swapChainBuffer;
		_swapChain->GetBuffer(0, __uuidof(swapChainBuffer), reinterpret_cast<void**>(&swapChainBuffer));
		D3D11_TEXTURE2D_DESC bufferDesc;
		swapChainBuffer->GetDesc(&bufferDesc);
		swapChainBuffer->Release();

		ID3D11Texture2D* depthTexture;
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = NULL;
		depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthTextureDesc.Height = bufferDesc.Height;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.MiscFlags = 0;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.Width = bufferDesc.Width;


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
			640,
			480
		};

		_context->RSSetViewports(1, &vp);
		

		D3D11_RASTERIZER_DESC rs;
		rs.AntialiasedLineEnable = true;
		rs.CullMode = D3D11_CULL_NONE;
		rs.DepthBias = 0;
		rs.DepthBiasClamp = 0;
		rs.DepthClipEnable = false;
		rs.FillMode = D3D11_FILL_SOLID;
		rs.FrontCounterClockwise = false;
		rs.MultisampleEnable = true;
		rs.ScissorEnable = false;
		rs.SlopeScaledDepthBias = 0;
		
		ID3D11RasterizerState* _rs;
		_device->CreateRasterizerState(&rs, &_rs);
		_context->RSSetState(_rs);

}

TungstenWin32Renderer::~TungstenWin32Renderer()
{
	_device->Release();
}

ID3D11Device * Tungsten::TungstenWin32Renderer::Device()
{
	return _device;
}

ID3D11DeviceContext * Tungsten::TungstenWin32Renderer::Context()
{
	return _context;
}

void Tungsten::TungstenWin32Renderer::Present()
{
	_swapChain->Present(1, 0);
}

void Tungsten::TungstenWin32Renderer::Clear(float4 color)
{
	_context->ClearDepthStencilView(_depthStencil, D3D11_CLEAR_DEPTH, 1.0f, (UINT8)0.0f);
	float clearColor[4] = { color.x,color.y,color.z,color.y };
	_context->ClearRenderTargetView(_renderTarget, clearColor);
}

void Tungsten::TungstenWin32Renderer::ResetRenderTarget()
{
	_context->OMSetRenderTargets(1, &_renderTarget, _depthStencil);
	_context->OMSetDepthStencilState(_depthStencilState, 0);
}
