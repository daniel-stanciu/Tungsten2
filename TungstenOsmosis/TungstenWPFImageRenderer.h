#pragma once
#include "Tungsten/TungstenMath.h"
#include "Tungsten/IGraphicsService.h"

#using "PresentationFramework.dll"
#using "PresentationCore.dll"
#using "System.Drawing.dll"
#using "WindowsBase.dll"

using namespace System::Windows::Controls;
using namespace System::Windows::Media::Imaging;
namespace Tungsten
{
	class TungstenWPFImageRenderer : public IGraphicsService
	{
	private:
		ID3D11Device * _device;
		ID3D11DeviceContext* _context;
		IDXGISwapChain1* _swapChain;
		ID3D11RenderTargetView* _renderTarget;
		ID3D11DepthStencilView* _depthStencil;
		ID3D11DepthStencilState* _depthStencilState;

		ID3D11Texture2D* _backBuffer;
		ID3D11Texture2D* _staging;
		
	public:
		TungstenWPFImageRenderer(Image^ image);
		// Inherited via IGraphicsService
		virtual ID3D11Device * Device() override;
		virtual ID3D11DeviceContext * Context() override;
		virtual void Present() override;

		// Inherited via IGraphicsService
		virtual void Clear(float4 color = { 0.0f,0.0f,0.0f,1.0f }) override;
		virtual void ResetRenderTarget() override;

		void Project(Image^ target);
	};
}


