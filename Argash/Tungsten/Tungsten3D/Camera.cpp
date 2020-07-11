#include "Camera.h"
#include "../ServiceLocator.h"
using namespace DirectX;
using namespace Tungsten;
Camera::Camera(void)
{
	lookAt=XMVectorSet(0.0f,0.0f,1.0f,1.0f);
	position=XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
	up=XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	perspective.x=0.0f;
	perspective.y=0.0f;

	D3D11_BUFFER_DESC  bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(matrix);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = NULL;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	Services.Graphics()->Device()->CreateBuffer(&bufferDesc, nullptr, &_cameraBuffer);
}


Camera::~Camera(void)
{
}

void Camera::SetLookAt(float x,float y,float z)
{
	
	lookAt=XMVectorSet(x,y,z,0.0f);
}

void Camera::SetPosition(float x,float y,float z)
{

	position=XMVectorSet(x,y,z,0.0f);
}

void Camera::SetUp(float x,float y, float z)
{
	up=XMVectorSet(x,y,z,0.0f);
}

void Camera::UpdateCameraMatrix()
{

	auto view = XMMatrixLookToLH(position,lookAt, up);
	auto perspective = XMMatrixPerspectiveFovLH(XM_PI*90.0f/180.0f ,4.0f/3.0f,0.1f,100.0f);
	matrix = view * perspective;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto _context = Services.Graphics()->Context();
	_context->Map(_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &matrix, sizeof(matrix));
	_context->Unmap(_cameraBuffer, 0);
	
}

ID3D11Buffer* Camera::GetCameraBuffer()
{
	return _cameraBuffer;
}

void Camera::SetPerspectiveDelta(float x,float y)
{
	/*
	This function receives the rotation of the perspective vector, as radians.
	*/
	perspective.x+=x;
	perspective.x=fmodf(perspective.x,XM_2PI);
	perspective.y+=y;
	if(perspective.y>XM_PIDIV2) perspective.y=XM_PIDIV2-0.001f;
	if(perspective.y<-XM_PIDIV2) perspective.y=-XM_PIDIV2+0.001f;
	
	XMMATRIX rotation=XMMatrixRotationRollPitchYaw(perspective.y,perspective.x,0);
	auto identity = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR cameraOffset=XMLoadFloat4(&identity);
	lookAt=XMVector4Transform(cameraOffset,rotation);
}

Vector<3,float> Camera::GetPosition()
{
	Vector<3,float> vector;
	vector.x=XMVectorGetX(position);
	vector.y=XMVectorGetY(position);
	vector.z=XMVectorGetZ(position);
	return vector;
}

void Camera::MoveRelative(float x, float y, float z)
{
	XMMATRIX rotation=XMMatrixRotationRollPitchYaw(perspective.y,perspective.x,0);
	XMVECTOR movement=XMLoadFloat4(&XMFLOAT4(x,y,z,0.0f));
	movement=XMVector4Transform(movement,rotation);
	//Keep perspective bound to gound.
	//movement=XMVectorSetY(movement,0);
	
	position=position+movement;
	
	

}

XMMATRIX Camera::GetLookAt()
{
	return XMMatrixLookAtLH(XMVectorSet(0.0f,0.0f,0.0f,0.0f),position+lookAt,up);
}