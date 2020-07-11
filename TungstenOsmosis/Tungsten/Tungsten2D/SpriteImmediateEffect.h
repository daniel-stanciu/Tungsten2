#pragma once
#include "../Tungsten3D/Effect.h"
#include "Sprite.h"
namespace Tungsten
{
	class SpriteImmediateEffect : public Effect
	{
	
	protected:
		ID3D11PixelShader * _PS;
		ID3D11VertexShader*	_VS;
		ID3D11InputLayout*	_IL;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		struct SpriteVertex
		{
			//Screen Position
			float4 pos;
			
			//Tint to change colour
			float4 tint;	
			//The position to be sampled from the texture
			float2 tex;
		};
		struct ConstantBuffer
		{
			float x;
			float y;
			float2 _pad = { 0.0f,0.0f };
		} _textureDimensions, _surfaceDimensions;
		ID3D11Buffer* _textureDimensionsBuffer;
		ID3D11Buffer* _surfaceDimensionsBuffer;
		ID3D11SamplerState* _samplerState;
		
	private:
		SpriteVertex _vertices[4];
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
	public:
	
		SpriteImmediateEffect();
		~SpriteImmediateEffect();

		// Inherited via Effect
		virtual void Apply() override;
		virtual void Draw() override;
		void SetSprrite(Sprite);
		void DrawSprite(Sprite);
		

	};

}