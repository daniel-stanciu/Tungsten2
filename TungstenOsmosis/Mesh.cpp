#include "pch.h"
#include "Mesh.h"

using namespace TungstenOsmosis;
Mesh::Mesh()
{
	_impl = new Tungsten::Mesh();
}

TungstenOsmosis::Mesh::Mesh(Tungsten::Mesh arg)
{
	_impl = new Tungsten::Mesh();
	(*_impl) = arg;
}
