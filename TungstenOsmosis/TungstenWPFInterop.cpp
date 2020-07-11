// This is the main DLL file.

#include "pch.h"

#include "TungstenWPFInterop.h"
#include "Tungsten/PlatformTools.h"
#include "Tungsten/ConfigManager.h"
#include <vcclr.h>  
#include "Tungsten\D3DRenderer.h"
#include "TungstenWPFWindowRenderer.h"
#include "Tungsten/ServiceLocator.h"
#include "Tungsten/TungstenMath.h"
#include "TungstenWPFImageRenderer.h"
using namespace System::IO;
using namespace Tungsten;
using namespace System::Windows;
using namespace System::Runtime::InteropServices;

void TungstenOsmosis::TungstenWPFInterop::InitialiseRendererFromWindow(Window^ window)
{
	auto wih = gcnew WindowInteropHelper(window);
	IntPtr hwnd = wih->Handle;	
	_graphics = new TungstenWPFWindowRenderer((HWND)hwnd.ToInt64(),window->Width,window->Height);
	Services.SetGraphics(_graphics);
	/*_sprite = new SpriteImmediateEffect();


	sp = new Sprite();


	sp->Position = { { 300_px,300_px },{ 300_px,500_px } };
	sp->Texture = TextureLoader::Get(L"Data\\Textures\\null.png");
	Tungsten::Rectangle<Pixels> ret = { { 0.0f,0.0f },{ 256.0f,256.0f } };

	sp->TextureSample = Tungsten::Rectangle<Pixels>::FromTopLeftSize(Tungsten::Point<Pixels>(0_px, 0_px), Tungsten::Point<Pixels>(256_px, 256_px));*/
}

void TungstenOsmosis::TungstenWPFInterop::InitialiseRendererFromHwnd(IntPtr hwnd,int width,int height)
{
	
	_graphics = new TungstenWPFWindowRenderer((HWND)hwnd.ToInt64(),width,height);
	Services.SetGraphics(_graphics);
	/*_sprite = new SpriteImmediateEffect();




	sp = new Sprite();


	sp->Position = { { 300_px,300_px },{ 600_px,600_px } };
	sp->Texture = TextureLoader::Get(L"Data\\Textures\\null.png");
	Tungsten::Rectangle<Pixels> ret = { { 0.0f,0.0f },{ 256.0f,256.0f } };

	sp->TextureSample = Tungsten::Rectangle<Pixels>::FromTopLeftSize(Tungsten::Point<Pixels>(0_px, 0_px), Tungsten::Point<Pixels>(256_px, 256_px));*/
}



void TungstenOsmosis::TungstenWPFInterop::InitialiseRendererFromImage(Image^ image)
{
	_graphics = new TungstenWPFImageRenderer(image);
	Services.SetGraphics(_graphics);
	/*_sprite = new SpriteImmediateEffect();


	sp = new Sprite();


	sp->Position = { { 300_px,300_px },{ 300_px,500_px } };
	sp->Texture = TextureLoader::Get(L"Data\\Textures\\null.png");
	Tungsten::Rectangle<Pixels> ret = { { 0.0f,0.0f },{ 256.0f,256.0f } };

	sp->TextureSample = Tungsten::Rectangle<Pixels>::FromTopLeftSize(Tungsten::Point<Pixels>(0_px, 0_px), Tungsten::Point<Pixels>(256_px, 256_px));

*/
}


TungstenOsmosis::TungstenWPFInterop::TungstenWPFInterop()
{
	

}

void TungstenOsmosis::TungstenWPFInterop::RenderOnImage(Image ^ image)
{
	_graphics->Present();
	auto imageRender = (TungstenWPFImageRenderer*)_graphics;
	imageRender->Project(image);
}

void TungstenOsmosis::TungstenWPFInterop::RenderOnWindow()
{



	//static float x = 0;
	//x = x + 0.3f;
	//if (x > 1)
	//	x -= 1;

	//sp->Position.Center.x += 2.0f;
	//if (sp->Position.Center.x > 500)
	//{
	//	sp->Position.Center.y += 5;
	//	sp->Position.Center.x = 0;
	//	if (sp->Position.Center.y > 500)
	//	{
	//		sp->Position.Center.y = 0;
	//	}
	//}
	//_graphics->Clear({ 0.0f,0.0f,1.0f,1.0f });
	//_sprite->DrawSprite(*sp);
	////sp->Position
	_graphics->Present();
}

void TungstenOsmosis::TungstenWPFInterop::Clear()
{
	_graphics->Clear({ 0.0f,0.0f,0.0f,1.0f });
}
