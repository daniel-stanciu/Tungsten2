#pragma once
#include "Effect.h"
#include "Mesh.h"
#include "../D3DRenderer.h"
#include "Camera.h"
#include "Texture.h"
#include "LightSource.h"

namespace Tungsten
{
	class MeshEffect : public Effect
	{
	private:
		Mesh * mesh;
		DirectX::XMMATRIX _worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX _position = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX _rotation = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX _scaling = DirectX::XMMatrixIdentity();

		Camera* camera;
		Texture* _texture;
		LightSource* _light;

		ID3D11PixelShader * _PS;
		ID3D11VertexShader*	_VS;
		ID3D11InputLayout*	_IL;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		ID3D11SamplerState* _samplerState;

		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
		ID3D11Buffer* _worldBuffer;
	public:
		MeshEffect();
		~MeshEffect();
		void Apply();
		void Draw();
		void SetMesh(Mesh* mesh);
		Mesh* GetMesh();
		
		void SetPosition(DirectX::XMMATRIX arg);
		void SetPosition(float x, float y, float z);

		void SetRotation(DirectX::XMMATRIX arg);
		void SetRotation(float x,float y,float z);

		void SetScaling(DirectX::XMMATRIX arg);
		void SetScaling(float x, float y, float z);

		void UpdateWorldMatrix();
		void SetTexture(Texture* texture);
		void SetLight(LightSource* light);
		Camera* GetCamera();
		LightSource* GetLight();
		DirectX::XMMATRIX GetWorldMatrix();
		Texture* GetTexture();
		void SetCamera(Camera* camera);
	};
}

