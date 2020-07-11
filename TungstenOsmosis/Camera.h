#pragma once
#include "Tungsten\Tungsten3D.h"
namespace TungstenOsmosis
{
	public ref class Camera
	{
	internal:
		Camera(Tungsten::Camera* arg);
		Tungsten::Camera* _impl;
	public:
		Camera();
		
	};
}

