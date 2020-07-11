#pragma once
#include "../D3DRenderer.h"
#include <vector>
#include "../PlatformTools.h"
#include "Mesh.h"

namespace Tungsten
{
	class OBJparser
	{
	private:
		int CheckOrientation(MeshVertex* data);
	public:

		void Parse(const WCHAR* path, const WCHAR* outputPath, int orientation = +1);
		Mesh Load(const WCHAR* path);

		OBJparser(void);
		~OBJparser(void);
	};
}
