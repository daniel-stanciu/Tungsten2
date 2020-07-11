#pragma once
#include "Mesh.h"
#include "Tungsten\Tungsten3D.h"
namespace TungstenOsmosis
{
	public ref class MeshLoader
	{
	protected:
		Tungsten::OBJparser* _impl;
	public:
		void Parse(System::String^ path, System::String^ outputPath);
		Mesh^ Load(System::String^ path);
		MeshLoader();
	};

}