#include "pch.h"
#include "Camera.h"
#include "Tungsten\Tungsten3D.h"

using namespace Tungsten;
using namespace TungstenOsmosis;

TungstenOsmosis::Camera::Camera(Tungsten::Camera* arg)
{
	this->_impl = arg;
	_impl->UpdateCameraMatrix();
}

TungstenOsmosis::Camera::Camera()
{
	_impl = new Tungsten::Camera();
	_impl->SetPosition(0, 0, -1.3);
	_impl->UpdateCameraMatrix();
}
