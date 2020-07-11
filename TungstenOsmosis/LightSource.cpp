#include "pch.h"
#include "LightSource.h"

using namespace TungstenOsmosis;
LightSource::LightSource()
{
	_impl = new Tungsten::LightSource();
	_impl->parameters.ambient = { 1.0f,1.0f,1.0f };
	_impl->parameters.diffuse = { 1.0f,1.0f,1.0f };
	_impl->parameters.direction = { 0.0f,0.0f,1.0f };
	DirectionZ = 1;
	_impl->UpdateBuffer();
	
}

TungstenOsmosis::LightSource::LightSource(Tungsten::LightSource * arg)
{
	_impl = arg;
}

void TungstenOsmosis::LightSource::SetAmbient(float r, float g, float b)
{
	_impl->parameters.ambient = { r,g,b };
	_impl->UpdateBuffer();
}

void TungstenOsmosis::LightSource::SetDiffuse(float r, float g, float b)
{
	_impl->parameters.diffuse = { r,g,b };
	_impl->UpdateBuffer();

}

void TungstenOsmosis::LightSource::SetDirection(float x, float y, float z)
{
	_impl->parameters.direction = { x,y,z };
	_impl->UpdateBuffer();
}
