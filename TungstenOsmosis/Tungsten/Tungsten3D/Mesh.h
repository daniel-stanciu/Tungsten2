#pragma once
#include "../PlatformTools.h"

namespace Tungsten
{
	class Mesh
	{
	private:

	public:
		Tungsten::MemoryBlock vertex;
		Tungsten::MemoryBlock index;
		Mesh();
		~Mesh();
	};

	struct MeshVertex
	{
		float pos[4];
		float texture[2];
		float normal[3];
	};
}
