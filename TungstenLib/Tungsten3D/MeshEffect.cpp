

#include "MeshEffect.h"
#include "../ServiceLocator.h"

using namespace Tungsten;

Tungsten::MeshEffect::MeshEffect()
{
	MemoryBlock bytes;
	_device = Tungsten::Services.Graphics()->Device();
	_context = Tungsten::Services.Graphics()->Context();

	

	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\PSSimpleModel.cso", &bytes);
	_device->CreatePixelShader(bytes.ptr, bytes.width, NULL, &_PS);
	PlatformTools::CreateByteCodeFromFile(L"Data\\Shaders\\VSSimpleModel.cso", &bytes);
	_device->CreateVertexShader(bytes.ptr, bytes.width, NULL, &_VS);
	D3D11_INPUT_ELEMENT_DESC IL_desc[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
		{ "TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,NULL }
	};
	_device->CreateInputLayout(IL_desc, 3, bytes.ptr, bytes.width, &_IL);

	D3D11_SAMPLER_DESC samplerDesc1;
	samplerDesc1.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc1.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc1.BorderColor[0] = 1.0f;
	samplerDesc1.BorderColor[1] = 1.0f;
	samplerDesc1.BorderColor[2] = 1.0f;
	samplerDesc1.BorderColor[3] = 1.0f;
	samplerDesc1.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc1.MaxAnisotropy = 1;
	samplerDesc1.MaxLOD = FLT_MAX;
	samplerDesc1.MinLOD = -FLT_MAX;
	samplerDesc1.MipLODBias = 0;

	_device->CreateSamplerState(&samplerDesc1, &_samplerState);
	_context->PSSetSamplers(0, 1, &_samplerState);
	_samplerState->Release();


	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(_worldMatrix);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = NULL;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_device->CreateBuffer(&bufferDesc, nullptr, &_worldBuffer);
	
	


}


Tungsten::MeshEffect::~MeshEffect()
{
}

void Tungsten::MeshEffect::Apply()
{
	_context->PSSetShader(_PS, NULL, NULL);
	_context->VSSetShader(_VS, NULL, NULL);
	_context->IASetInputLayout(_IL);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT offset = 0;
	UINT vertexStride = sizeof(MeshVertex);
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexStride, &offset);
	_context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	auto cameraBuffer = camera->GetCameraBuffer();
	_context->VSSetConstantBuffers(0, 1, &cameraBuffer);
	_context->VSSetConstantBuffers(1, 1, &_worldBuffer);
	_context->PSSetShaderResources(0, 1, &_texture->shaderView);
	_context->PSSetConstantBuffers(0, 1, &_light->buffer);
	UpdateWorldMatrix();
}

void Tungsten::MeshEffect::Draw()
{
	_context->DrawIndexed(mesh->index.width / sizeof(UINT32), 0, 0);
}

void Tungsten::MeshEffect::SetMesh(Mesh * mesh)
{
	this->mesh = mesh;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.MiscFlags = NULL;
	vertexBufferDesc.StructureByteStride = NULL;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;




	D3D11_BUFFER_DESC indexDesc;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = NULL;
	indexDesc.MiscFlags = NULL;
	indexDesc.StructureByteStride = NULL;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;



	//VertexBuffer
	vertexBufferDesc.ByteWidth = mesh->vertex.width;

	D3D11_SUBRESOURCE_DATA vertex_data;
	vertex_data.pSysMem = mesh->vertex.ptr;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	_device->CreateBuffer(&vertexBufferDesc, &vertex_data, &_vertexBuffer);

	//IndexBuffer
	indexDesc.ByteWidth = mesh->index.width;

	D3D11_SUBRESOURCE_DATA index_data;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;
	index_data.pSysMem = mesh->index.ptr;

	_device->CreateBuffer(&indexDesc, &index_data, &_indexBuffer);



}

Mesh * Tungsten::MeshEffect::GetMesh()
{
	return mesh;
}

void Tungsten::MeshEffect::SetPosition(DirectX::XMMATRIX arg)
{
	_position = arg;
	UpdateWorldMatrix();
}

void Tungsten::MeshEffect::SetPosition(float x, float y, float z)
{
	auto arg = DirectX::XMMatrixTranslation(x, y, z);
	SetPosition(arg);
}

void Tungsten::MeshEffect::SetRotation(DirectX::XMMATRIX arg)
{
	_rotation = arg;
	UpdateWorldMatrix();
}

void Tungsten::MeshEffect::SetRotation(float x, float y, float z)
{
	auto arg = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	SetRotation(arg);
}

void Tungsten::MeshEffect::SetScaling(DirectX::XMMATRIX arg)
{
	_scaling = arg;
	UpdateWorldMatrix();
}


void Tungsten::MeshEffect::SetScaling(float x,float y,float z)
{
	auto arg = DirectX::XMMatrixScaling(x, y, z);
	SetScaling(arg);
}


void Tungsten::MeshEffect::UpdateWorldMatrix()
{
	_worldMatrix = _scaling * _rotation*_position;
	RemapConstantBuffer(_worldBuffer, _worldMatrix);
}



void Tungsten::MeshEffect::SetTexture(Texture * texture)
{
	this->_texture = texture;
}

void Tungsten::MeshEffect::SetLight(LightSource * light)
{
	this->_light = light;
}

Tungsten::Camera* Tungsten::MeshEffect::GetCamera()
{
	return camera;
}

LightSource * Tungsten::MeshEffect::GetLight()
{
	return _light;
}

DirectX::XMMATRIX Tungsten::MeshEffect::GetWorldMatrix()
{
	return _worldMatrix;
}

Texture * Tungsten::MeshEffect::GetTexture()
{
	return _texture;
}

void Tungsten::MeshEffect::SetCamera(Camera * camera)
{
	this->camera = camera;
}
