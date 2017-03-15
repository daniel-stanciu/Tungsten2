#pragma once
#include "pch.h"
#include <vector>
#include <Tungsten\DataContainers.h>
struct SimpleVertex
{
	float pos[4];
	float texture[2];
	float normal[3];
};
struct Model
{
	DirectX::XMMATRIX worldMatrix;
	Tungsten::MemoryBlock vertex;
	Tungsten::MemoryBlock index;
	
	
};
class OBJparser
{
private:
	int CheckOrientation(SimpleVertex* data);
public:
	
	void Parse(WCHAR* path,WCHAR* outputPath,int orientation=+1);
	Model Load(WCHAR* path);
	void Draw(Model& model);
	
	OBJparser(void);
	~OBJparser(void);
};

