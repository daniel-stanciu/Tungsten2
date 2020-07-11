#pragma once
#include "../DataContainers.h"
#include "../TungstenMath.h"
#include <d3d11.h>
namespace Tungsten
{
	struct CameraBuffer
	{
		DirectX::XMMATRIX view;
	};
	class Camera
	{
	private:
		Tungsten::Vector<2, float> perspective;

		__declspec(align(16))	DirectX::XMVECTOR lookAt;
		__declspec(align(16))	DirectX::XMVECTOR position;
		__declspec(align(16))	DirectX::XMVECTOR up;
		__declspec(align(16))	DirectX::XMMATRIX matrix;
		ID3D11Buffer* _cameraBuffer;
	public:
		Camera(void);
		~Camera(void);
		void			UpdateCameraMatrix();
		ID3D11Buffer*	GetCameraBuffer();
		DirectX::XMMATRIX		GetLookAt();
		void					SetLookAt(float x, float y, float z);
		void					SetPosition(float x, float y, float z);
		void					SetUp(float x, float y, float z);
		//Pass relative Deltas
		void					SetPerspectiveDelta(float x, float y);
		void					MoveRelative(float x, float y, float z);
		Tungsten::
			Vector<3, float>	GetPosition();
	};
}