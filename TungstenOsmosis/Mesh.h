#pragma once
#include "Tungsten\Tungsten3D.h"
namespace TungstenOsmosis
{
	public ref class Mesh
	{
	internal :
		Tungsten::Mesh* _impl;
	public:
		Mesh();
		Mesh(Tungsten::Mesh arg);
	};
}


