
#include "OBJparser.h"
#include "..\PlatformTools.h"
#include "..\DataContainers.h"
#include <vector>
#include "..\ServiceLocator.h"
#include "Mesh.h"
using namespace Tungsten;
using namespace std;
using namespace DirectX;
OBJparser::OBJparser(void)
{
}


OBJparser::~OBJparser(void)
{
}

void OBJparser::Parse(const WCHAR* path,const WCHAR* outputPath,int orientation)
{
	std::vector<MeshVertex> vertexData;
	UINT32* indexData;
	int faceCount;
	stringstream* stream;
	PlatformTools::CreateStringBufferFromFile(path,&stream);
	char line[100];
	stringstream token;
	stringbuf face;
	float aux;
	int index;
	int lineNo=0;
	faceCount=0;
	MeshVertex auxVertex;
	vector<float> normal;
	vector<float> position;
	vector<float> texture;
	//int orientation=+1;

	while(!stream->eof())
	{
 		stream->getline(line,100);
		lineNo++;
		if(stream->fail() && !stream->eof() ) 
		{
			wchar_t error[100];
			swprintf_s(error,100,L"Error parsing line%d",lineNo);
			PlatformTools::PostError(error);
			return;
		}
		//Compose vertices data
		if(line[0]=='v')
		{
			switch(line[1])
			{
			case ' ' : 
				{
					token=stringstream(line+2);
					token>>aux;
					position.push_back(aux);
					token>>aux;
					position.push_back(aux);
					token>>aux;
					position.push_back(aux);
					//default w component to 1;
					aux=1.0f;
					token>>aux;
					position.push_back(aux);
					break;
				}
			case 'n' : 
				{
					token=stringstream(line+2);
					token>>aux;
					normal.push_back(aux);
					token>>aux;
					normal.push_back(aux);
					token>>aux;
					normal.push_back(aux);
					
					break;
				}
				
			case 't' :
				{
					token=stringstream(line+2);
					token>>aux;
					texture.push_back(aux);
					token>>aux;
					texture.push_back(aux);
				}
			}
		}
		//Compose faces
		if(line[0]=='f')
		{
			token=stringstream(line+2);
			for(int i=0;i<3;i++)
			{
				
				//position
				token>>index;
				
				if (index < 0)
				{
					index = position.size() + index;
				}
				else
				{
					index--;
				}
				auxVertex.pos[0]=position[4*index+0];
				auxVertex.pos[1]=position[4*index+1];
				auxVertex.pos[2]=orientation*position[4*index+2];
				/*auxVertex.pos[2]=position[4*index+2];*/
				auxVertex.pos[3]=position[4*index+3];
				token.ignore();
				//texture
				token>>index;
				index--;
				if(texture.size())
				{
					auxVertex.texture[0]=texture[2*index+0];
					if(orientation<0)
					auxVertex.texture[1]=1-texture[2*index+1];
					else
					auxVertex.texture[1]=texture[2*index+1];
				}
				else
				{
					auxVertex.texture[0]=0;
					auxVertex.texture[1]=0;
				}

				token.ignore();
				//normal
				token>>index;
				index--;
				auxVertex.normal[0]=normal[3*index+0];
				auxVertex.normal[1]=normal[3*index+1];
				auxVertex.normal[2]=orientation*normal[3*index+2];
				/*auxVertex.normal[2]=normal[3*index+2];*/
				token.ignore();
				
				vertexData.push_back(auxVertex);
			}
			/*if(!faceCount)
			{
				orientation=CheckOrientation(vertexData.data());
			}*/
			faceCount++;
		}
		if(line[0]=='#')
		{
			//stream->ignore();
		}
		if(/*line[0]==' ' ||*/ line[0]==0)
		{
			stream->ignore();
		}
	}
	indexData=new UINT32[faceCount*3];
	if(orientation>0)
	{
		for(int i=0;i<faceCount*3;i++)
		{
			indexData[i]=i;
			
		}
	}
	if(orientation<0)
	{
		for(int i=0;i<faceCount*3;i+=3)
		{
			indexData[i]=i;
			indexData[i+1]=i+2;
			indexData[i+2]=i+1;
			
		}
	}
	
	HANDLE output=PlatformTools::CreateFileHandle(outputPath,PlatformTools::Create | PlatformTools::Write);
	//FileFormat
	/*
	int-NumberOfBytes indexData
	bytes-IndexData
	int-NumberOfBytes vertexData
	bytes-IndexData
	*/
	faceCount*=3*sizeof(UINT32);
	WriteFile(output,&faceCount,sizeof(faceCount),NULL, NULL);
	WriteFile(output,indexData,faceCount,NULL, NULL);

	//Size of the vertex buffer
	faceCount=(int)sizeof(MeshVertex)*vertexData.size();
	WriteFile(output,&faceCount,sizeof(faceCount),NULL, NULL);
	WriteFile(output,vertexData.data(),faceCount,NULL, NULL);
	
	CloseHandle(output);
}


Mesh OBJparser::Load(const WCHAR* path)
{
	
	
	
	HANDLE file=PlatformTools::CreateFileHandle(path,PlatformTools::Read);
	Mesh ret;

	ReadFile(file, &ret.index.width, sizeof(int),NULL,NULL);
	ret.index.ptr=new byte[ret.index.width];
	ReadFile(file, ret.index.ptr, ret.index.width,NULL,NULL);

	ReadFile(file, &ret.vertex.width, sizeof(int),NULL,NULL);
	ret.vertex.ptr=new byte[ret.vertex.width];
	ReadFile(file, ret.vertex.ptr, ret.vertex.width,NULL,NULL);

	return ret;
}

int OBJparser::CheckOrientation(MeshVertex* data)
{
	int handedness=0;
	XMVECTOR vector[2];
	
	
	vector[0]=XMLoadFloat3((XMFLOAT3*)&data[0].pos);
	vector[1]=XMLoadFloat3((XMFLOAT3*)&data[1].pos);
	vector[0]=vector[1]-vector[0];
	vector[1]=vector[1]-XMLoadFloat3((XMFLOAT3*)&data[3].pos);
	vector[0]=XMVector3Cross(vector[0],vector[1]);
	//handedness=XMVectorGetByIndex(vector[0],2);
	if(handedness<0) handedness=-1;
	if(handedness>0) handedness=+1;
	//Update Z
	data[0].pos[2]*=handedness;
	data[1].pos[2]*=handedness;
	data[2].pos[2]*=handedness;
	//Update V
	data[0].texture[1]*=handedness;
	data[1].texture[1]*=handedness;
	data[2].texture[1]*=handedness;
	//Update normal Z
	data[0].normal[2]*=handedness;
	data[1].normal[2]*=handedness;
	data[2].normal[2]*=handedness;
	return handedness;
}