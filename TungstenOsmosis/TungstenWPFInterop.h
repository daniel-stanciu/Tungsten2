// TungstenOsmosis.h

#pragma once

#pragma unmanaged
#include "Tungsten/IGraphicsService.h"
#include "Tungsten/Tungsten2D.h"
#pragma managed

#using "PresentationFramework.dll"
#using "PresentationCore.dll"
#using "System.Drawing.dll"
#using "WindowsBase.dll"

using namespace System;
using namespace System::Windows::Interop;
using namespace System::Windows::Controls;
using namespace System::Windows;
namespace TungstenOsmosis {

	public ref class TungstenWPFInterop
	{
	private:
		Tungsten::IGraphicsService* _graphics;
		Tungsten::SpriteImmediateEffect* _sprite;
		
		
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGISwapChain1* swapChain;

		Tungsten::Sprite* sp;
	public:
		void InitialiseRendererFromWindow(Window^ window);
		void InitialiseRendererFromHwnd(IntPtr hwnd,int width, int height);
		void InitialiseRendererFromImage(Image^ image);
		TungstenWPFInterop();
		void RenderOnImage(Image^ image);
		void RenderOnWindow();
		void Clear();
	};
}
