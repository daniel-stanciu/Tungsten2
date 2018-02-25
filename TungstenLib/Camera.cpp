#include "pch.h"
#include "Camera.h"
#include <math.h>
using namespace DirectX;
using namespace Tungsten;
Camera::Camera(void)
{
	
	lookAt=XMVectorSet(0.0f,0.0f,1.0f,1.0f);
	position=XMVectorSet(0.0f,0.0f,-1.0f,0.0f);
	up=XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	perspective.x=0.0f;
	perspective.y=0.0f;
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

DirectX::XMMATRIX* Camera::GetCameraMatrix()
{

	matrix=XMMatrixLookAtLH(position,position+lookAt,up)*XMMatrixPerspectiveFovLH(XM_PI*90.0f/180.0f ,4.0f/3.0f,1.0f,1000.0f);
	return &matrix;
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
	XMVECTOR cameraOffset=XMLoadFloat4(&XMFLOAT4(0.0f,0.0f,1.0f,0.0f));
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