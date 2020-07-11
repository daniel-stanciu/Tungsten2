#include "LightSource.h"
#include "../ServiceLocator.h"

using namespace Tungsten;

LightSource::LightSource()
{
	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(parameters);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = NULL;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	Services.Graphics()->Device()->CreateBuffer(&bufferDesc, nullptr, &buffer);
	UpdateBuffer();
}


LightSource::~LightSource()
{
}

void Tungsten::LightSource::UpdateBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto _context = Services.Graphics()->Context();
	Services.Graphics()->Context()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &parameters, sizeof(parameters));
	Services.Graphics()->Context()->Unmap(buffer, 0);
}