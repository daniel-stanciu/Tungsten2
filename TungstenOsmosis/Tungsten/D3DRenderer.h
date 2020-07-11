//#pragma once
//
//#include "Tungsten3D/Texture.h"
//#include "DirectXTK\WICTextureLoader.h"
//#include "Timer.h"
//#include "PlatformTools.h"
//#include "DataContainers.h"
//#include "Camera.h"
//#include "IGraphicsService.h"
//#include "ServiceLocator.h"
//#ifdef WIN32
//#include <d3d11_4.h>
//#endif
//#ifdef WINRT
//#include <d3d11_1.h>
//#endif
//namespace Tungsten
//{
//
//	class D3DRenderer: public IGraphicsService
//	{
//	private:
//		ID3D11Debug* debug;
//		float								g_width;
//	    float								g_height;
//		float								g_dpi;
//		struct 
//		{
//			ID3D11PixelShader*	PS;
//			ID3D11VertexShader*	VS;
//			ID3D11InputLayout*	IA;
//		}effects[10];
//		//Active Blend state
//		ID3D11BlendState* blendState;
//		ID3D11Device*						g_pd3dDevice;
//		
//		ID3D11DeviceContext*				g_pContext;
//		D3D11_BUFFER_DESC vertexBufferDesc;
//		UINT vertexStride;
//		D3D11_BUFFER_DESC indexDesc;
//		IDXGISwapChain1*					g_pdxgiSwapChain;
//		ID3D11RenderTargetView*				g_prenderTargetView;
//		ID3D11DepthStencilView*				depthStencilView;
//		IDXGISurface*						dxgiBuffer;							
//		void InitialiseStates();
//	
//	public:
//		D3DRenderer(ID3D11Device* device, IDXGISwapChain1* swapChain);
//		//IGraphicsService interface
//		virtual void	Refresh() ;
//		virtual void	SetBuffers(MemoryBlock vertexData,MemoryBlock indexData, MemoryBlock constantData=MemoryBlock::Null());
//		virtual void	DrawIndexed(UINT IndexCount,UINT StartIndexLocation,INT BaseVertexLocation);
//		virtual void	SetTexture(Texture);
//		virtual Texture LoadTexture(const WCHAR* filepath);
//		virtual void	Present();
//		virtual void	SetState(GraphicsState);
//		virtual void	SetConstantBufferVS(MemoryBlock buffer,int index);
//		virtual void	SetConstantBufferPS(MemoryBlock buffer,int index);
//		virtual Texture	LoadSkyMap();
//		virtual void	DrawInstanced(MemoryBlock vertexData,MemoryBlock indexData,MemoryBlock instanceData);
//
//		void Clear(float clearcolor[4]);
//		Camera* GetCamera();
//		virtual ID3D11Device * Device() override;
//
//		virtual ID3D11DeviceContext* Context() override;
//		
//		
//		
//		~D3DRenderer(void);
//	private:
//		Timer animationTimer;
//		float ConvertDpisToPixels(float dips);
//	    void CreateContextResources();
//	    void CreateDeviceResources();
//		
//		
//
//		// Inherited via IGraphicsService
//		
//
//	};
//	
//}
//	
//	