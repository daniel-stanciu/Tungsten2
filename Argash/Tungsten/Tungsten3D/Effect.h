#pragma once
#include <d3d11_1.h>
#include "../ServiceLocator.h"
namespace Tungsten
{
	class Effect
	{
	protected:
		template<typename T> void RemapConstantBuffer(ID3D11Buffer* buffer,const T& arg);
	public:
		Effect();
		virtual void Apply() =0;
		virtual void Draw() =0;
		virtual ~Effect() = 0;
	};


	template<typename T>
	inline void Effect::RemapConstantBuffer(ID3D11Buffer * buffer,const T& arg)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		auto _context = Services.Graphics()->Context();
		_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &arg, sizeof(arg));
		_context->Unmap(buffer, 0);
	}

}

