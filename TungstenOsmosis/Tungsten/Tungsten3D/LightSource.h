#pragma once
#include "../TungstenMath.h"
#include <d3d11.h>
namespace Tungsten
{
	class LightSource
	{
	private:

	public:
		struct
		{
			_declspec(align(16)) Vector<3, float> direction;
			float _pad;
			_declspec(align(16)) Vector<3, float> diffuse;
			float _pad1;
			_declspec(align(16)) Vector<3, float> ambient;
			float _pad2;
			Vector<4,float> _pad3;
		}parameters;
		ID3D11Buffer* buffer;
		void UpdateBuffer();
		LightSource();
		~LightSource();
	};
}
