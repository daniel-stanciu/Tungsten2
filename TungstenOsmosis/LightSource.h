#pragma once
#include "Tungsten\Tungsten3D.h"
namespace TungstenOsmosis
{
	public ref class LightSource
	{
	internal:
		Tungsten::LightSource * _impl;
		float _directionZ;
		float _directionY;
		float _directionX;
	public:
		LightSource();
		LightSource(Tungsten::LightSource * arg);

		void SetAmbient(float r, float g, float b);
		void SetDiffuse(float r, float g, float b);
		void SetDirection(float x, float y, float z);
		//Direction
		
		property float DirectionX {
			float get() { return _directionX; }
			void set(float value) { _directionX = value; SetDirection(_directionX, _directionY, _directionZ); }
		}

		
		property float DirectionY {
			float get() { return _directionY; }
			void set(float value) {
				_directionY = value; SetDirection(_directionX, _directionY, _directionZ);
			}
		}

		
		property float DirectionZ {
			float get() { return _directionZ; }
			void set(float value) {
				_directionZ = value;
				SetDirection(_directionX, _directionY, _directionZ);
			}
		}
	};

}

