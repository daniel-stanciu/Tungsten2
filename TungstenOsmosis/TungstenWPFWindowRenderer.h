#pragma once
#include "Tungsten/TungstenMath.h"
#include "Tungsten/IGraphicsService.h"
namespace Tungsten
{

	class TungstenWPFWindowRenderer : public IGraphicsService
	{
	private:
		ID3D11Device * _device;
		ID3D11DeviceContext* _context;
		IDXGISwapChain* _swapChain;
		ID3D11RenderTargetView* _renderTarget;
		ID3D11DepthStencilView* _depthStencil;
		ID3D11DepthStencilState* _depthStencilState;
	public:
		TungstenWPFWindowRenderer(HWND hwnd,int width,int height);
		~TungstenWPFWindowRenderer();

		// Inherited via IGraphicsService
		virtual ID3D11Device * Device() override;
		virtual ID3D11DeviceContext * Context() override;
		virtual void Present() override;

		// Inherited via IGraphicsService
		virtual void Clear(float4 color = { 0.0f,0.0f,0.0f,1.0f }) override;
		virtual void ResetRenderTarget() override;
	};

}

