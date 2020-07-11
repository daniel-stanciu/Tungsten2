#include "D3DRenderer.h"
#include "DirectXTK\DDSTextureLoader.h"
using namespace DirectX;
using namespace Tungsten;
using namespace Tungsten::PlatformTools;

void D3DRenderer::Clear(float clearcolor[4])
{
	
	g_pContext->ClearRenderTargetView(g_prenderTargetView,clearcolor);
	//Curatam cu valoarea 1.0f , valoarea maxim posibila
	g_pContext->ClearDepthStencilView(depthStencilView,D3D11_CLEAR_DEPTH,1.0f,(UINT8)0.0f);
	//g_pdxgiSwapChain->Present(0,0);
	//g_pdxgiSwapChain->Present(0, 0);
	RECT rect;
	rect.bottom = 200;
	rect.right = 200;
	rect.top = 100;
	rect.left = 100;
	DXGI_PRESENT_PARAMETERS params;
	params.DirtyRectsCount = 1;
	params.pDirtyRects = &rect;
	params.pScrollOffset = nullptr;
	params.pScrollRect = nullptr;
	g_pdxgiSwapChain->Present1(0, 0, &params);
}


void Tungsten::D3DRenderer::InitialiseStates()
{
	
	ID3D11Texture2D* swapChainBuffer;
	g_pdxgiSwapChain->GetBuffer(0, __uuidof(swapChainBuffer), reinterpret_cast<void**>(&swapChainBuffer) );
	D3D11_TEXTURE2D_DESC bufferDesc;
	swapChainBuffer->GetDesc(&bufferDesc);
	swapChainBuffer->Release();

	UINT sampleLevels[10];
	g_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 1, sampleLevels);
	//g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&debug));
	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ID3D11Texture2D* depthTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = NULL;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.Height = bufferDesc.Height;
	depthTextureDesc.Height = 
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.MiscFlags = 0;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Width = bufferDesc.Width;
	CheckFatalError(g_pd3dDevice->CreateTexture2D(&depthTextureDesc, NULL, &depthTexture));
	CheckFatalError(g_pd3dDevice->CreateDepthStencilView(depthTexture, NULL, &depthStencilView));
	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.StencilEnable = false;
	ID3D11DepthStencilState* depthState;
	CheckFatalError(g_pd3dDevice->CreateDepthStencilState(&depthStateDesc, &depthState));
	g_pContext->OMSetDepthStencilState(depthState, NULL);
	//TODO:Remove this
	ID3D11Texture2D* backbuffer;
	g_pdxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
	g_pd3dDevice->CreateRenderTargetView(backbuffer, NULL, &g_prenderTargetView);

	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		(float)bufferDesc.Width,
		(float)bufferDesc.Height
	);
	g_pContext->RSSetViewports(1, &viewport);

	//Shader Loading

	//SpriteRender State

//	PlatformTools::CheckFatalError(g_pd3dDevice->CreateInputLayout(IA_desc, 3, bytes.ptr, bytes.width, &effects[0].IA));

	//SimpleModel State
	MemoryBlock bytes;
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSimpleModel.cso", &bytes);
	g_pd3dDevice->CreatePixelShader(bytes.ptr, bytes.width, NULL, &effects[1].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSimpleModel.cso", &bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr, bytes.width, NULL, &effects[1].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc2[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL }
	};
//	PlatformTools::CheckFatalError(g_pd3dDevice->CreateInputLayout(IA_desc2, 3, bytes.ptr, bytes.width, &effects[1].IA));

	//SkyMap State

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSkyBox.cso", &bytes);
	g_pd3dDevice->CreatePixelShader(bytes.ptr, bytes.width, NULL, &effects[2].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSkyBox.cso", &bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr, bytes.width, NULL, &effects[2].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc3[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL }
	};
//	PlatformTools::CheckFatalError(g_pd3dDevice->CreateInputLayout(IA_desc2, 3, bytes.ptr, bytes.width, &effects[2].IA));

	//Grass State

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSInstancedGrass.cso", &bytes);
	g_pd3dDevice->CreatePixelShader(bytes.ptr, bytes.width, NULL, &effects[3].PS);

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSInstancedGrass.cso", &bytes);
	g_pd3dDevice->CreateVertexShader(bytes.ptr, bytes.width, NULL, &effects[3].VS);

	D3D11_INPUT_ELEMENT_DESC IA_desc4[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
	{ "IPOSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1 }
	};
//	PlatformTools::CheckFatalError(g_pd3dDevice->CreateInputLayout(IA_desc4, 4, bytes.ptr, bytes.width, &effects[3].IA));
}

Tungsten::D3DRenderer::D3DRenderer(ID3D11Device * device, IDXGISwapChain1 * swapChain)
{
	this->g_pd3dDevice = device;
	this->g_pdxgiSwapChain = swapChain;
	device->GetImmediateContext(&g_pContext);
	InitialiseStates();
	float color[4] = { 0.0f,0.0f,1.0f,1.0f };
	this->Clear(color);
	g_pdxgiSwapChain->Present(0, 0);
}



D3DRenderer::~D3DRenderer(void)
{
}

ID3D11Device * Tungsten::D3DRenderer::Device()
{
	return this->g_pd3dDevice;
}

ID3D11DeviceContext* Tungsten::D3DRenderer::Context()
{
	return this->g_pContext;
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

Texture D3DRenderer::LoadTexture(const WCHAR* filepath)
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