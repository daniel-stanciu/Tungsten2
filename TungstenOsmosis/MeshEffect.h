#pragma once
#include "Tungsten\Tungsten3D.h"
#include "Tungsten\ServiceLocator.h"
#include "Camera.h"
#include "IEffect.h"
#include "Mesh.h"
#include "LightSource.h"
#include "Texture.h"
namespace TungstenOsmosis
{
	public ref class MeshEffect : public  IEffect
	{
	private:
		Tungsten::MeshEffect *_impl;
		LightSource^ light;
	public:
		MeshEffect();
		property Camera^ Camera {
			TungstenOsmosis::Camera^ get()
			{
				return gcnew TungstenOsmosis::Camera(_impl->GetCamera());
			}
			void set(TungstenOsmosis::Camera^ value)
			{
				_impl->SetCamera(value->_impl);
			}
		}
		property TungstenOsmosis::Mesh^ Mesh {
			TungstenOsmosis::Mesh^ get() {
				auto ret = gcnew TungstenOsmosis::Mesh();
				ret->_impl = _impl->GetMesh();
				return ret;
			}
			void set(TungstenOsmosis::Mesh^ value)
			{
				_impl->SetMesh(value->_impl);
			}
		}
		property TungstenOsmosis::LightSource^ LightSource {
			TungstenOsmosis::LightSource^ get() {
				return light;
			}
			void set(TungstenOsmosis::LightSource^ value) {
				_impl->SetLight(value->_impl);
				light = value;
			}
		}

		property TungstenOsmosis::Texture^ Texture {
			TungstenOsmosis::Texture^ get() {
				return gcnew TungstenOsmosis::Texture(_impl->GetTexture());
			}
			void set(TungstenOsmosis::Texture^ value) {
				_impl->SetTexture(value->_impl);
			}
		}
		virtual void Draw();
		virtual void Apply();

		//Rotation
		float _rotationX;
		property float RotationX {
			float get() { return _rotationX; }
			void set(float value) { _rotationX = value; _impl->SetRotation(_rotationX, _rotationY, _rotationZ); }
		}

		float _rotationY;
		property float RotationY {
			float get() { return _rotationY; }
			void set(float value) { _rotationY = value; _impl->SetRotation(_rotationX, _rotationY, _rotationZ);
			}
		}

		float _rotationZ;
		property float RotationZ {
			float get() { return _rotationZ; }
			void set(float value) { _rotationZ = value; _impl->SetRotation(_rotationX, _rotationY, _rotationZ);
			}
		}

		//Scaling

		float _scalingX=1;
		property float ScalingX {
			float get() { return _scalingX; }
			void set(float value) { _scalingX = value; _impl->SetScaling(_scalingX, _scalingY, _scalingZ); }
		}

		float _scalingY=1;
		property float ScalingY {
			float get() { return _scalingY; }
			void set(float value) {
				_scalingY = value; _impl->SetScaling(_scalingX, _scalingY, _scalingZ);
			}
		}

		float _scalingZ=1;
		property float ScalingZ {
			float get() { return _scalingZ; }
			void set(float value) {
				_scalingZ = value; _impl->SetScaling(_scalingX, _scalingY, _scalingZ);
			}
		}

		//Translation

		float _positionX;
		property float PositionX {
			float get() { return _positionX; }
			void set(float value) { _positionX = value; _impl->SetPosition(_positionX, _positionY, _positionZ); }
		}

		float _positionY;
		property float PositionY {
			float get() { return _positionY; }
			void set(float value) {
				_positionY = value; _impl->SetPosition(_positionX, _positionY, _positionZ);
			}
		}

		float _positionZ;
		property float PositionZ {
			float get() { return _positionZ; }
			void set(float value) {
				_positionZ = value; _impl->SetPosition(_positionX, _positionY, _positionZ);
			}
		}

	};

}

