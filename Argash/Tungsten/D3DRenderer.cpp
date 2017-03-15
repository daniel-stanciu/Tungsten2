#include "pch.h"
#include "D3DRenderer.h"
#include "Tungsten\DirectXTK\DDSTextureLoader.h"
using namespace DirectX;
using namespace Tungsten;
using namespace Tungsten::PlatformTools;
#if defined WINRT
D3DRenderer::D3DRenderer(	Windows::UI::Core::CoreWindow^								m_coreWindow,
					Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^		m_swapChain)
{
	//Preluam interfata nativa a obiectului din mediul "managed"
	reinterpret_cast<IUnknown*>(m_swapChain)->QueryInterface(__uuidof(ISwapChainBackgroundPanelNative),(void**)&g_SwapChainBackgroundPanelNnative);
	g_dpi=Windows::Graphics::Display::DisplayProperties::LogicalDpi;
	//Convertim DotsPerInh in pixeli
	g_width=ConvertDpisToPixels(m_coreWindow->Bounds.Width);
	g_height=ConvertDpisToPixels(m_coreWindow->Bounds.Height);
	//Initializam Device,Context si SwapChain
	CreateDeviceResources();
	CreateContextResources();
}


float D3DRenderer::ConvertDpisToPixels(float dips)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * g_dpi / dipsPerInch + 0.5f); // Round to nearest integer.
}

void D3DRenderer::CreateContextResources()
{
	
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Vertex Shader
	//!# HARDCODED!
	void* buffer;
	int size;
	Tungsten::PlatformTools::CreateByteCodeFromFile(L"Assets\\VertexShader.cso",&buffer,&size);
	//Platform::Array<byte>^ data_array=Tungsten::PlatformTools::GetBytecodeFromFile(L"Assets\\VertexShader.cso");
	g_pd3dDevice->CreateVertexShader(buffer,size,nullptr,&p_vertexShader);
	//g_pd3dDevice->CreateVertexShader(data_array->Data,data_array->Length,nullptr,&p_vertexShader);
	D3D11_INPUT_ELEMENT_DESC description[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	ID3D11InputLayout* inputLayout=nullptr;
	g_pd3dDevice->CreateInputLayout(description,ARRAYSIZE(description),buffer,size,&inputLayout);
	//	size,&inputLayout);
	g_pContext->IASetInputLayout(inputLayout);
	g_pContext->VSSetShader(p_vertexShader,nullptr,0);
	//Pixel Shader
	//!# HARDCODED!
	Tungsten::PlatformTools::CreateByteCodeFromFile(L"Assets\\PixelShader.cso",&buffer,&size);
	g_pd3dDevice->CreatePixelShader(buffer,size,nullptr,&p_pixelShader);
	g_pContext->PSSetShader(p_pixelShader,nullptr,0);
	D3D11_SAMPLER_DESC sample_desc;
	ZeroMemory(&sample_desc,sizeof(sample_desc));
	sample_desc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
	ID3D11SamplerState* sampler_state;
	g_pd3dDevice->CreateSamplerState(&sample_desc,&sampler_state);
	g_pContext->PSSetSamplers(0,1,&sampler_state);
	
}

void D3DRenderer::CreateDeviceResources()
{
	
	UINT creationflags;
	creationflags=D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	creationflags|=D3D11_CREATE_DEVICE_DEBUG;
	
	D3D_FEATURE_LEVEL farray[]=
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	
	D3D11CreateDevice(
		nullptr,
		
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationflags,
		farray,
		ARRAYSIZE(farray),
		D3D11_SDK_VERSION,
		&g_pd3dDevice,
		nullptr,
		&g_pContext);
	//Obtinem IDXGIFactory2 din D3D11_Device
	IDXGIDevice1* dxgidev;
	reinterpret_cast<IUnknown*>(g_pd3dDevice)->QueryInterface(__uuidof(IDXGIDevice1),(void**)&dxgidev);
	IDXGIAdapter* dxgiAdapt;
	dxgidev->GetAdapter(&dxgiAdapt);
	IDXGIFactory2* dxgifact;
	dxgiAdapt->GetParent(__uuidof(IDXGIFactory2),(void**)&dxgifact);
	//Umplem struct de tip Description pentru D3D11SwapChain ce va fi creat
	DXGI_SWAP_CHAIN_DESC1 sc_desc;
	//Dimensiune, am caclucalt-o cu ConvertDipsToPixels
	sc_desc.Width=(UINT) g_width;
	sc_desc.Height=(UINT) g_height;
	//Formatul texturii, cate 8 biti pentru fiecare culoare a fiecarui pixel
	sc_desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	//Functionalitate D3D11.1 efect 3D sterescopic (pentru ochelari 3D)
	sc_desc.Stereo=false;
	//Multisampling
	sc_desc.SampleDesc.Count=1;
	sc_desc.SampleDesc.Quality=0;
	//Scoupul bufferelor
	sc_desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//Numarul bufferelor
	sc_desc.BufferCount=4;
	sc_desc.Scaling= DXGI_SCALING_STRETCH;
	sc_desc.SwapEffect=DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sc_desc.AlphaMode=DXGI_ALPHA_MODE_IGNORE;
	sc_desc.Flags=0;
	dxgifact->CreateSwapChainForComposition(g_pd3dDevice,
		&sc_desc,
		nullptr,
		&g_pdxgiSwapChain);

	//Set up the depth stencil
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (int)g_width;
	descDepth.Height = (int)g_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	g_pd3dDevice ->CreateTexture2D( &descDepth, NULL, &pDepthStencil ) ;
	
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	//Depth test
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//Comparam daca valoarea de adancime este mai mica decat cea prezenta deja
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	// Stencil - ignoram
	dsDesc.StencilEnable = false;


	// Creem starea care explica ce reguli se folosesc, relativ redundant cand folosim doar partea de "depth"
	ID3D11DepthStencilState * pDSState;

	g_pd3dDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	
	//
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags=0;

	// Creem View
	g_pd3dDevice->CreateDepthStencilView( pDepthStencil,
		&descDSV, 
		&g_pdepthStencilView );  
	// Setam starea DS
	
	g_pContext->OMSetDepthStencilState(pDSState, 1);
	
	//Render Target
	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetView(
		D3D11_RTV_DIMENSION_TEXTURE2DARRAY,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0,
		0,
		1
	);
	ID3D11Texture2D* backbuffer;
		g_pdxgiSwapChain->GetBuffer(0,IID_PPV_ARGS(&backbuffer));
	//Zona din ecran pe care va fi desenat, pornint de la 0,0 pana la g_widtch,g_height
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		g_width,
		g_height
		);
	g_pContext->RSSetViewports(1,&viewport);
	
	g_pd3dDevice->CreateRenderTargetView(backbuffer,&renderTargetView,&g_prenderTargetView);
	// Setam DS view
	g_pContext->OMSetRenderTargets( 1,          
		&g_prenderTargetView,      
		g_pdepthStencilView );     
	//#!
	HRESULT hr=
	g_SwapChainBackgroundPanelNnative->SetSwapChain(g_pdxgiSwapChain);

	//Clear(Windows::UI::Colors::Blue);
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc,sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable=true;
	blendDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
 
	ID3D11BlendState* blendState;
	g_pd3dDevice->CreateBlendState(&blendDesc,&blendState);
	g_pContext->OMSetBlendState(blendState,NULL,0xffffff);
}
#endif
void D3DRenderer::Clear(float clearcolor[4])
{
	
	g_pContext->ClearRenderTargetView(g_prenderTargetView,clearcolor);
	//Curatam cu valoarea 1.0f , valoarea maxim posibila
	g_pContext->ClearDepthStencilView(depthStencilView,D3D11_CLEAR_DEPTH,1.0f,(UINT8)0.0f);
	//g_pdxgiSwapChain->Present(0,0);
	
}

#if defined WIN32
D3DRenderer::D3DRenderer(HWND	h_window)
{
	ConfigurationState config=Services.Config()->Get();
	UINT flags=0;
	flags|=D3D11_CREATE_DEVICE_SINGLETHREADED; //D3D calls will only come from one thread
	flags|=D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//required for D2D interop
#if defined _DEBUG
	flags|=D3D11_CREATE_DEVICE_DEBUG;
#endif
	DXGI_SWAP_CHAIN_DESC scDescription;
	scDescription.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	scDescription.BufferDesc.Height=config.resolution.height;
	scDescription.BufferDesc.Width=config.resolution.width;
	DXGI_RATIONAL refreshRate={0,0};	//Use active refresh rate
	scDescription.BufferDesc.RefreshRate=refreshRate;
	//Default values, black magic inside
	scDescription.BufferDesc.Scaling=DXGI_MODE_SCALING_UNSPECIFIED;
	scDescription.BufferDesc.ScanlineOrdering=DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	scDescription.BufferCount=2;
	scDescription.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDescription.Flags=0;
	scDescription.OutputWindow=h_window;
	scDescription.SampleDesc.Count=1;
	scDescription.SampleDesc.Quality=0;
	scDescription.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
	scDescription.Windowed=true;
	if(config.windowed==config.no)
		scDescription.Windowed=false;

	D3D11CreateDeviceAndSwapChain(
		NULL,						//Use default adapter
		D3D_DRIVER_TYPE_HARDWARE,	//Use hardware renderer
		NULL,						//Sofware DLL, not used
		flags,
		NULL,						//Uses default feature level array D3D_FEATURE_LEVEL_11_0 to 9_1
		NULL,						//Number of feature levels, use default
		D3D11_SDK_VERSION,
		&scDescription,
		&g_pdxgiSwapChain,
		&g_pd3dDevice,
		NULL,
		&g_pContext
		);
	UINT sampleLevels[10];
	g_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,1,sampleLevels);
	//g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&debug));
	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ID3D11Texture2D* depthTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.ArraySize=1;
	depthTextureDesc.BindFlags=D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags=NULL;
	depthTextureDesc.Format=DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.Height=config.resolution.height;
	depthTextureDesc.MipLevels=1;
	depthTextureDesc.MiscFlags=0;
	depthTextureDesc.SampleDesc.Count=1;
	depthTextureDesc.SampleDesc.Quality=0;
	depthTextureDesc.Usage=D3D11_USAGE_DEFAULT;
	depthTextureDesc.Width=config.resolution.width;
	CheckFatalError(g_pd3dDevice->CreateTexture2D(&depthTextureDesc,NULL,&depthTexture));
	CheckFatalError( g_pd3dDevice->CreateDepthStencilView(depthTexture,NULL,&depthStencilView) );
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	depthStateDesc.DepthEnable=true;
	depthStateDesc.DepthFunc=D3D11_COMPARISON_LESS_EQUAL;
	depthStateDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.StencilEnable=false;
	ID3D11DepthStencilState* depthState;
	CheckFatalError( g_pd3dDevice->CreateDepthStencilState(&depthStateDesc,&depthState) );
	g_pContext->OMSetDepthStencilState(depthState,NULL);
	//TODO:Remove this
	ID3D11Texture2D* backbuffer;
	g_pdxgiSwapChain->GetBuffer(0,IID_PPV_ARGS(&backbuffer));
	g_pd3dDevice->CreateRenderTargetView(backbuffer,NULL,&g_prenderTargetView);
	/*D3D11_RASTERIZER_DESC rasteriser;
	rasteriser.FillMode=D3D11_FILL_SOLID;
	rasteriser.CullMode=D3D11_CULL_NONE;
	rasteriser.FrontCounterClockwise=false;
	rasteriser.DepthBias=0.0f;
	rasteriser.SlopeScaledDepthBias=0.0f;
	rasteriser.DepthBiasClamp=0.0f;
	rasteriser.DepthClipEnable=true;
	rasteriser.ScissorEnable=false;
	rasteriser.MultisampleEnable=false;
	rasteriser.AntialiasedLineEnable=false;
	ID3D11RasterizerState* rasteriserState;
	g_pd3dDevice->CreateRasterizerState(&rasteriser,&rasteriserState);
	g_pContext->RSSetState(rasteriserState);*/
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		(float)config.resolution.width,
		(float)config.resolution.height
		);
	g_pContext->RSSetViewports(1,&viewport);

	//Shader Loading

	//SpriteRender State
	MemoryBlock bytes;
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSpriteRender.cso",&bytes);
	g_pd3dDevice->CreatePixelShader(bytes.ptr,bytes.width ,NULL,&effects[0].PS);
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSpriteRender.cso",&bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr,bytes.width ,NULL,&effects[0].VS);
	D3D11_INPUT_ELEMENT_DESC IA_desc[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"TINT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL}
	};
	PlatformTools::CheckFatalError( g_pd3dDevice->CreateInputLayout(IA_desc,3,bytes.ptr,bytes.width ,&effects[0].IA) );

	//SimpleModel State

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSimpleModel.cso",&bytes );
	g_pd3dDevice->CreatePixelShader(bytes.ptr,bytes.width ,NULL,&effects[1].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSimpleModel.cso",&bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr,bytes.width ,NULL,&effects[1].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc2[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL}
	};
	PlatformTools::CheckFatalError( g_pd3dDevice->CreateInputLayout(IA_desc2,3,bytes.ptr,bytes.width ,&effects[1].IA) );

	//SkyMap State

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSkyBox.cso",&bytes );
	g_pd3dDevice->CreatePixelShader(bytes.ptr,bytes.width ,NULL,&effects[2].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSkyBox.cso",&bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr,bytes.width ,NULL,&effects[2].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc3[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL}
	};
	PlatformTools::CheckFatalError( g_pd3dDevice->CreateInputLayout(IA_desc2,3,bytes.ptr,bytes.width ,&effects[2].IA) );

	//Grass State

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSInstancedGrass.cso",&bytes );
	g_pd3dDevice->CreatePixelShader(bytes.ptr,bytes.width ,NULL,&effects[3].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSInstancedGrass.cso",&bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr,bytes.width ,NULL,&effects[3].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc4[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL},
		{"IPOSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1}
	};
	PlatformTools::CheckFatalError( g_pd3dDevice->CreateInputLayout(IA_desc4,4,bytes.ptr,bytes.width ,&effects[3].IA) );


	float color[4]={0.0f,0.0f,1.0f,1.0f};
	this->Clear(color);
	g_pdxgiSwapChain->Present(0,0);
};

#endif
/*
void D3DRenderer::RenderNextAnimationFrame()
{
	/*
	g_pContext->OMSetRenderTargets(1,&g_prenderTargetView,g_pdepthStencilView);
	MemoryBlock vertexData;
	MemoryBlock indexData;
	spriteCache->GetData(&vertexData,&indexData);
	//VertexBuffer
	D3D11_BUFFER_DESC vertex_buffer_desc;
	vertex_buffer_desc.ByteWidth=			vertexData.Width;
	vertex_buffer_desc.BindFlags=			D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags=		NULL;
	vertex_buffer_desc.MiscFlags=			NULL;
	vertex_buffer_desc.StructureByteStride=	sizeof(Vertex);
	vertex_buffer_desc.Usage=				D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA vertex_data;
	vertex_data.pSysMem=vertexData.ptr;
	vertex_data.SysMemPitch=0;
	vertex_data.SysMemSlicePitch=0;
	ID3D11Buffer* p_vbuffer;
	g_pd3dDevice->CreateBuffer(&vertex_buffer_desc,&vertex_data,&p_vbuffer);
	//IndexBuffer
	D3D11_BUFFER_DESC index_desc;
	index_desc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	index_desc.ByteWidth=indexData.Width;
	index_desc.CPUAccessFlags=NULL;
	index_desc.MiscFlags=NULL;
	index_desc.StructureByteStride=sizeof(Index);
	index_desc.Usage=D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA index_data;
	index_data.SysMemPitch=0;
	index_data.SysMemSlicePitch=0;
	index_data.pSysMem=indexData.ptr;
	ID3D11Buffer* p_indexBuffer;
	g_pd3dDevice->CreateBuffer(&index_desc,&index_data,&p_indexBuffer);
	UINT stride=sizeof(Vertex);
	UINT offset=0;
	g_pContext->IASetVertexBuffers(0,1,&p_vbuffer,&stride,&offset);
	g_pContext->IASetIndexBuffer(p_indexBuffer,DXGI_FORMAT_R32_UINT,offset);
	SpriteBatch nextBatch=spriteCache->GetNextBatch();
	while(nextBatch.sprite_count)
	{
		g_pContext->PSSetShaderResources(0,1,&nextBatch.p_texture);
		g_pContext->DrawIndexed(nextBatch.sprite_count*6,nextBatch.previous_sprites*6,0);
		//g_pContext->DrawIndexed(12,0,0);
		nextBatch=spriteCache->GetNextBatch();
	}
	
	
}
*/

D3DRenderer::~D3DRenderer(void)
{
}

Camera* D3DRenderer::GetCamera()
{
	return NULL;
}

//IGraphicsService implementations
void D3DRenderer::Refresh()
{
	//g_prenderTargetView->Release();
	
	g_pContext->OMSetRenderTargets(1,&g_prenderTargetView,depthStencilView);
	float color[4];
	color[0]=0.0f;
	color[1]=0.0f;
	color[2]=1.0f;
	color[3]=1.0f;
	Clear(color);
}

void D3DRenderer::SetBuffers(MemoryBlock vertexData,MemoryBlock indexData, MemoryBlock constantData)
{
	//VertexBuffer
	vertexBufferDesc.ByteWidth=vertexData.width;

	D3D11_SUBRESOURCE_DATA vertex_data;
	vertex_data.pSysMem=vertexData.ptr;
	vertex_data.SysMemPitch=0;
	vertex_data.SysMemSlicePitch=0;
	ID3D11Buffer* p_vbuffer;
	g_pd3dDevice->CreateBuffer(&vertexBufferDesc,&vertex_data,&p_vbuffer);
	
	//IndexBuffer
	indexDesc.ByteWidth=indexData.width;
	
	D3D11_SUBRESOURCE_DATA index_data;
	index_data.SysMemPitch=0;
	index_data.SysMemSlicePitch=0;
	index_data.pSysMem=indexData.ptr;
	ID3D11Buffer* p_indexBuffer;
	g_pd3dDevice->CreateBuffer(&indexDesc,&index_data,&p_indexBuffer);

	UINT offset=0;
	g_pContext->IASetVertexBuffers(0,1,&p_vbuffer,&vertexStride,&offset);
	g_pContext->IASetIndexBuffer(p_indexBuffer,DXGI_FORMAT_R32_UINT,offset);
	p_vbuffer->Release();
	p_indexBuffer->Release();
}

void D3DRenderer::DrawIndexed(UINT IndexCount,UINT StartIndexLocation,INT BaseVertexLocation)
{
	g_pContext->DrawIndexed(IndexCount,StartIndexLocation,BaseVertexLocation);
}

void D3DRenderer::SetTexture(Texture texture)
{
	g_pContext->PSSetShaderResources(0,1,&texture.shaderView);
}

Texture D3DRenderer::LoadTexture(WCHAR* filepath)
{
	Texture ret;
	CreateWICTextureFromFile(this->g_pd3dDevice,this->g_pContext,filepath,&ret.resource,&ret.shaderView);
	return ret;
}

void D3DRenderer::Present()
{
	g_pdxgiSwapChain->GetBuffer(0,IID_PPV_ARGS(&dxgiBuffer));
	g_pdxgiSwapChain->Present(0,0);
	dxgiBuffer->Release();
}

void D3DRenderer::SetState(GraphicsState state)
{
	switch (state)
	{
		case SpriteRender : 
			vertexBufferDesc.BindFlags=				D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags=		NULL;
			vertexBufferDesc.MiscFlags=				NULL;
			vertexBufferDesc.StructureByteStride=	NULL;
			vertexBufferDesc.Usage=					D3D11_USAGE_DEFAULT;

			vertexStride=sizeof(SpriteVertex);

			indexDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
			indexDesc.CPUAccessFlags=NULL;
			indexDesc.MiscFlags=NULL;
			indexDesc.StructureByteStride=NULL;
			indexDesc.Usage=D3D11_USAGE_DEFAULT;

			

			g_pContext->PSSetShader(effects[0].PS,NULL,NULL);
			g_pContext->VSSetShader(effects[0].VS,NULL,NULL);
			g_pContext->IASetInputLayout(effects[0].IA);

			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.ComparisonFunc=D3D11_COMPARISON_NEVER;
			samplerDesc.BorderColor[0]=1.0f;
			samplerDesc.BorderColor[1]=1.0f;
			samplerDesc.BorderColor[2]=1.0f;
			samplerDesc.BorderColor[3]=1.0f;
			samplerDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.MaxAnisotropy=1;
			samplerDesc.MaxLOD=FLT_MAX;
			samplerDesc.MinLOD=-FLT_MAX;
			samplerDesc.MipLODBias=0;
			ID3D11SamplerState* samplerState;
			g_pd3dDevice->CreateSamplerState(&samplerDesc,&samplerState);
			g_pContext->PSSetSamplers(0,1,&samplerState);
			samplerState->Release();

			D3D11_BLEND_DESC blendDesc;
			blendDesc.AlphaToCoverageEnable=false;
			blendDesc.IndependentBlendEnable=false;
			blendDesc.RenderTarget[0].BlendEnable=true;
			blendDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
			
			g_pd3dDevice->CreateBlendState(&blendDesc,&blendState);
			g_pContext->OMSetBlendState(blendState,NULL,0xffffffff);
						
			break;
		case SimpleModel :
			D3D11_SAMPLER_DESC samplerDesc1;
			samplerDesc1.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc1.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc1.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc1.ComparisonFunc=D3D11_COMPARISON_NEVER;
			samplerDesc1.BorderColor[0]=1.0f;
			samplerDesc1.BorderColor[1]=1.0f;
			samplerDesc1.BorderColor[2]=1.0f;
			samplerDesc1.BorderColor[3]=1.0f;
			samplerDesc1.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc1.MaxAnisotropy=1;
			samplerDesc1.MaxLOD=FLT_MAX;
			samplerDesc1.MinLOD=-FLT_MAX;
			samplerDesc1.MipLODBias=0;
			ID3D11SamplerState* samplerState1;
			g_pd3dDevice->CreateSamplerState(&samplerDesc1,&samplerState1);
			g_pContext->PSSetSamplers(0,1,&samplerState1);
			samplerState1->Release();
			vertexBufferDesc.BindFlags=				D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags=		NULL;
			vertexBufferDesc.MiscFlags=				NULL;
			vertexBufferDesc.StructureByteStride=	NULL;
			vertexBufferDesc.Usage=					D3D11_USAGE_DEFAULT;

			vertexStride=sizeof(float)*9;

			indexDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
			indexDesc.CPUAccessFlags=NULL;
			indexDesc.MiscFlags=NULL;
			indexDesc.StructureByteStride=NULL;
			indexDesc.Usage=D3D11_USAGE_DEFAULT;

			

			g_pContext->PSSetShader(effects[1].PS,NULL,NULL);
			g_pContext->VSSetShader(effects[1].VS,NULL,NULL);
			g_pContext->IASetInputLayout(effects[1].IA);
			break;
		case SkyBox :
			D3D11_SAMPLER_DESC samplerDesc2;
			samplerDesc2.AddressU=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc2.AddressV=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc2.AddressW=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc2.ComparisonFunc=D3D11_COMPARISON_NEVER;
			samplerDesc2.BorderColor[0]=1.0f;
			samplerDesc2.BorderColor[1]=1.0f;
			samplerDesc2.BorderColor[2]=1.0f;
			samplerDesc2.BorderColor[3]=1.0f;
			samplerDesc2.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc2.MaxAnisotropy=1;
			samplerDesc2.MaxLOD=FLT_MAX;
			samplerDesc2.MinLOD=-FLT_MAX;
			samplerDesc2.MipLODBias=0;
			ID3D11SamplerState* samplerState2;
			g_pd3dDevice->CreateSamplerState(&samplerDesc2,&samplerState2);
			g_pContext->PSSetSamplers(0,1,&samplerState2);
			samplerState2->Release();
			vertexBufferDesc.BindFlags=				D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags=		NULL;
			vertexBufferDesc.MiscFlags=				NULL;
			vertexBufferDesc.StructureByteStride=	NULL;
			vertexBufferDesc.Usage=					D3D11_USAGE_DEFAULT;

			vertexStride=sizeof(float)*9;

			indexDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
			indexDesc.CPUAccessFlags=NULL;
			indexDesc.MiscFlags=NULL;
			indexDesc.StructureByteStride=NULL;
			indexDesc.Usage=D3D11_USAGE_DEFAULT;

			

			g_pContext->PSSetShader(effects[2].PS,NULL,NULL);
			g_pContext->VSSetShader(effects[2].VS,NULL,NULL);
			g_pContext->IASetInputLayout(effects[2].IA);
			break;
		case Grass:
			D3D11_SAMPLER_DESC samplerDesc3;
			samplerDesc3.AddressU=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc3.AddressV=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc3.AddressW=D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc3.ComparisonFunc=D3D11_COMPARISON_NEVER;
			samplerDesc3.BorderColor[0]=1.0f;
			samplerDesc3.BorderColor[1]=1.0f;
			samplerDesc3.BorderColor[2]=1.0f;
			samplerDesc3.BorderColor[3]=1.0f;
			samplerDesc3.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc3.MaxAnisotropy=1;
			samplerDesc3.MaxLOD=FLT_MAX;
			samplerDesc3.MinLOD=-FLT_MAX;
			samplerDesc3.MipLODBias=0;
			ID3D11SamplerState* samplerState3;
			g_pd3dDevice->CreateSamplerState(&samplerDesc3,&samplerState3);
			g_pContext->PSSetSamplers(0,1,&samplerState3);
			samplerState3->Release();
			vertexBufferDesc.BindFlags=				D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags=		NULL;
			vertexBufferDesc.MiscFlags=				NULL;
			vertexBufferDesc.StructureByteStride=	NULL;
			vertexBufferDesc.Usage=					D3D11_USAGE_DEFAULT;

			vertexStride=sizeof(float)*9;

			indexDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
			indexDesc.CPUAccessFlags=NULL;
			indexDesc.MiscFlags=NULL;
			indexDesc.StructureByteStride=NULL;
			indexDesc.Usage=D3D11_USAGE_DEFAULT;

			

			g_pContext->PSSetShader(effects[3].PS,NULL,NULL);
			g_pContext->VSSetShader(effects[3].VS,NULL,NULL);
			g_pContext->IASetInputLayout(effects[3].IA);
			break;

			
		default:
			break;
	}
}

void D3DRenderer::SetConstantBufferVS(MemoryBlock buffer,int index)
{
	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth=buffer.width;
	bufferDesc.CPUAccessFlags=NULL;
	bufferDesc.MiscFlags=NULL;
	bufferDesc.StructureByteStride=NULL;
	bufferDesc.Usage=D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem=buffer.ptr;
	bufferData.SysMemPitch=0;
	bufferData.SysMemSlicePitch=0;

	ID3D11Buffer* constantBuffer;
	g_pd3dDevice->CreateBuffer(&bufferDesc,&bufferData,&constantBuffer);
	g_pContext->VSSetConstantBuffers(index,1,&constantBuffer);
	constantBuffer->Release();
}

void D3DRenderer::SetConstantBufferPS(MemoryBlock buffer,int index)
{
	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	//Must be multiple of 16 bytes
	bufferDesc.ByteWidth=buffer.width;

	bufferDesc.CPUAccessFlags=NULL;
	bufferDesc.MiscFlags=NULL;
	bufferDesc.StructureByteStride=NULL;
	bufferDesc.Usage=D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem=buffer.ptr;
	bufferData.SysMemPitch=0;
	bufferData.SysMemSlicePitch=0;

	ID3D11Buffer* constantBuffer;
	g_pd3dDevice->CreateBuffer(&bufferDesc,&bufferData,&constantBuffer);
	g_pContext->PSSetConstantBuffers(index,1,&constantBuffer);
	constantBuffer->Release();
}

Texture D3DRenderer::LoadSkyMap()
{
	Texture tex;
	/*
	CreateWICTextureFromFile(g_pd3dDevice,g_pContext,L"Data\\Textures\\SkyBox\\grave_back.jpg",&tex.resource,&tex.shaderView);
	SkyMap ret;
	ret.textures[0]=NULL;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.ArraySize=6;
	texDesc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags=NULL;
	texDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Height=512;
	texDesc.MipLevels=1;
	texDesc.MiscFlags=NULL;
	texDesc.SampleDesc.Count=1;
	texDesc.SampleDesc.Quality=0;
	texDesc.Usage=D3D11_USAGE_DEFAULT;
	texDesc.Width=512;
	D3D11_SUBRESOURCE_DATA data[6];
	void* buffer;
	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_right.bmp",&buffer,NULL);
	data[0].pSysMem=buffer;
	data[0].SysMemPitch=512*4;
	data[0].SysMemSlicePitch=512*4*512;

	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_left.bmp",&buffer,NULL);
	data[1].pSysMem=buffer;
	data[1].SysMemPitch=512*4;
	data[1].SysMemSlicePitch=512*4*512;

	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_top.bmp",&buffer,NULL);
	data[2].pSysMem=buffer;
	data[2].SysMemPitch=512*4;
	data[2].SysMemSlicePitch=512*4*512;

	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_top.bmp",&buffer,NULL);
	data[3].pSysMem=buffer;
	data[3].SysMemPitch=512*4;
	data[3].SysMemSlicePitch=512*4*512;

	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_front.bmp",&buffer,NULL);
	data[4].pSysMem=buffer;
	data[4].SysMemPitch=512*4;
	data[4].SysMemSlicePitch=512*4*512;

	PlatformTools::CreateByteCodeFromFile(L"Data\\Textures\\SkyBox\\grave_back.bmp",&buffer,NULL);
	data[5].pSysMem=buffer;
	data[5].SysMemPitch=512*4;
	data[5].SysMemSlicePitch=512*4*512;

	ID3D11Texture2D* texture;
	g_pd3dDevice->CreateTexture2D(&texDesc,data,&texture);
	
	tex.resource=NULL;
	*/
	CreateDDSTextureFromFile(g_pd3dDevice,L"Data\\Textures\\imnotboxsky.dds",&tex.resource,&tex.shaderView);
	return tex;
}

void D3DRenderer::DrawInstanced(MemoryBlock vertexData,MemoryBlock indexData,MemoryBlock instanceData)
{
	//VertexBuffer
	vertexBufferDesc.ByteWidth=vertexData.width;

	D3D11_SUBRESOURCE_DATA vertex_data;
	vertex_data.pSysMem=vertexData.ptr;
	vertex_data.SysMemPitch=0;
	vertex_data.SysMemSlicePitch=0;
	ID3D11Buffer* buffers[2];
	g_pd3dDevice->CreateBuffer(&vertexBufferDesc,&vertex_data,&buffers[0]);
	
	//IndexBuffer
	indexDesc.ByteWidth=indexData.width;
	
	D3D11_SUBRESOURCE_DATA index_data;
	index_data.SysMemPitch=0;
	index_data.SysMemSlicePitch=0;
	index_data.pSysMem=indexData.ptr;
	ID3D11Buffer* p_indexBuffer;
	g_pd3dDevice->CreateBuffer(&indexDesc,&index_data,&p_indexBuffer);

	UINT offset=0;

	D3D11_BUFFER_DESC instanceBuffer;
	instanceBuffer.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	instanceBuffer.ByteWidth=instanceData.width;
	instanceBuffer.CPUAccessFlags=NULL;
	instanceBuffer.MiscFlags=NULL;
	instanceBuffer.StructureByteStride=NULL;
	instanceBuffer.Usage=D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA instance_data;
	instance_data.SysMemPitch=0;
	instance_data.SysMemSlicePitch=0;
	instance_data.pSysMem=instanceData.ptr;

	g_pd3dDevice->CreateBuffer(&instanceBuffer,&instance_data,&buffers[1]);

	UINT strides[2];
	strides[0]=vertexStride;
	strides[1]=sizeof(float)*3;
	
	UINT offsets[2];
	offsets[0]=0;
	offsets[1]=0;

	g_pContext->IASetVertexBuffers(0,2,buffers,strides,offsets);
	g_pContext->IASetIndexBuffer(p_indexBuffer,DXGI_FORMAT_R32_UINT,offset);

	g_pContext->DrawIndexedInstanced(6,100,0,0,0);
	buffers[0]->Release();
	buffers[1]->Release();
	p_indexBuffer->Release();
}