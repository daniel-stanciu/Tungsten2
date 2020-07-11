// Argash.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "Application.h"
#include "Tungsten\ConfigManager.h"
#include "version.h"
#include "Tungsten/Tungsten2D.h"
#include "Tungsten\Tungsten3D.h"
//#include "Tungsten\D3DRenderer.h"
#include "Tungsten\ServiceLocator.h"
#include "TungstenWin32/TungstenWin32Renderer.h"
#include "Tungsten/Timer.h"
//#include "Tungsten\SpriteTextService.h"
//#include "GameStateMachine.h"
//#include "OBJparser.h"

using namespace Tungsten;
using namespace DirectX;
#define MAX_LOADSTRING 100
#define MACRO_STRING_TOKEN(s) #s
#define MACRO_STRING(s)  MACRO_STRING_TOKEN(s)
//#define MACRO_WSTRING(s) L MACRO_STRING(s)
#define VERSION_NUMBER "v"  MACRO_STRING(VERSION_RELEASE_NUMBER) "." \
	 MACRO_STRING(VERSION_MAJOR_NUMBER) "." \
	 MACRO_STRING(VERSION_MINOR_NUMBER) "." \
	 MACRO_STRING(VERSION_BUILD_NUMBER)

// Global Variables:
HINSTANCE	g_currentInstance;					// Current process instance
HWND		g_mainWindow;						// Current window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//InputService input;
// Forward declarations of functions included in this code module:
WORD				RegisterMainWindow(HINSTANCE hInstance);
BOOL				InitMainWindow(HINSTANCE);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//
//struct SkyMap
//{
//	Texture* textures[6];
//};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	g_currentInstance = hInstance; // Store instance handle in our global variable
	MSG msg;
	/*Services.SetConfig(new ConfigManager());
	Services.Config()->Load(L"config.cfg");
	input.Initialise(g_mainWindow);*/
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	_tcscat_s(szTitle, L" ");
	char version[] = VERSION_NUMBER;
	//Abomination of Hell, don't disturb unless Vatar attacks again
	_tcscat_s(szTitle, std::wstring(version, version + strlen(version)).c_str());
	LoadString(hInstance, IDC_ARGASH, szWindowClass, MAX_LOADSTRING);
	RegisterMainWindow(hInstance);

	// Perform application initialization:
	if (!InitMainWindow(hInstance))
	{
		return FALSE;
	}




	PROCESS_MEMORY_COUNTERS_EX memory;

	
	Timer animationTimer;
	animationTimer.Start();

	Services.SetGraphics(new TungstenWin32Renderer(g_mainWindow));

	SpriteImmediateEffect spriteEffect;
	Sprite sp;
	sp.Position = { { 300_px,300_px }, {125_px,125_px} };
	sp.Texture = TextureLoader::Get(L"Data\\Textures\\null.png");
	Tungsten::Rectangle<Pixels> ret = { { 0.0f,0.0f },{ 256.0f,256.0f } };

	sp.TextureSample = Tungsten::Rectangle<Pixels>::FromTopLeftSize(Point<Pixels>(0_px, 0_px), Point<Pixels>(256_px, 256_px));
	auto texture = TextureLoader::Get(L"Data\\RUST_3d_Low1_Difuse.jpg");
	Camera camera;
	camera.SetPosition(0, 0,1);
	camera.SetLookAt(0,0, -1);
	camera.SetUp(0, 1, 0);
	//camera.UpdateCameraMatrix();

	LightSource light;
	light.parameters.direction = Vector<3, float>(0.0f, -2.0f, -2.5f);
	light.parameters.diffuse = Vector<3, float>(0.3f, 0.1f, 0.2f);
	light.parameters.ambient = Vector<3, float>(0.4f, 0.2f, 0.3f);
	light.UpdateBuffer();

	MeshEffect mesh;
	mesh.SetCamera(&camera);
	mesh.SetWorldMatrix(DirectX::XMMatrixIdentity());
	mesh.SetTexture(&texture);
	OBJparser parser;
	parser.Parse(L"Data\\RUST_3d_Low1.obj", L"Data\\RUST_3d_Low1.bmd");
	Mesh data = parser.Load(L"Data\\RUST_3d_Low1.bmd");
	mesh.SetMesh(&data);
	mesh.SetLight(&light);

	GetProcessMemoryInfo(GetCurrentProcess(), (PPROCESS_MEMORY_COUNTERS)&memory, sizeof(memory));
	while (true)
	{
		GetProcessMemoryInfo(GetCurrentProcess(), (PPROCESS_MEMORY_COUNTERS)&memory, sizeof(memory));
		//Services.Refresh();
		//config=Services.Config()->Get();
//		input.Reset();
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			animationTimer.Tick();
			auto color = animationTimer.GetTotalMs() %10000 / 10000.0f*3-1;
			camera.SetPosition(0, 0, 2);
			camera.UpdateCameraMatrix();
			Services.Graphics()->Clear({ 0.0f,0.0f,0.0f,1.0f });
			mesh.Apply();
			mesh.Draw();
		/*	spriteEffect.Apply();
			spriteEffect.DrawSprite(sp);*/
			Services.Graphics()->Present();




	}
	return 0;
	//return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
WORD RegisterMainWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	//	Tungsten::ConfigurationState state=Services.Config()->Get();
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARGASH));
	wcex.hCursor = 0;
	//wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	/*wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+4);*/
	//if(state.windowed==state.no)
	//	wcex.hbrBackground	= (HBRUSH)0;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	SetCursor(0);
	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitMainWindow(HINSTANCE hInstance)
{
	HWND hWnd = 0;
	/*Tungsten::ConfigurationState state = Services.Config()->Get();*/


	//// Set the window's style.
 //  if(state.windowed==state.no)
 //  {
	//   //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	//   RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	//   AdjustWindowRect(&rectangle,WS_POPUP,false);
	// //  HBRUSH brush = (HBRUSH)0;
	//  // SetClassLongPtr(g_mainWindow, GCLP_HBRBACKGROUND, (LONG)brush);
	//   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
	//	   200,200,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
 //  }
 //  if(state.windowed==state.borderless)
   //{
	  // //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	  // RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	  // //AdjustWindowRect(&rectangle,WS_POPUP,false);
	  // hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP ,
		 //  0,0,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
   //}
	// if(state.windowed==state.yes)
 //  {
	//   //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	//   RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	//   AdjustWindowRect(&rectangle,WS_POPUP,false);
	//   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION,
	//	   200,200,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
 //  }

 //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	RECT rectangle = { 0,0, 640,480 };
	//AdjustWindowRect(&rectangle,WS_POPUP,false);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
		0, 0, rectangle.right, rectangle.bottom, NULL, NULL, g_currentInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	// Maximize the window.
	ShowWindow(hWnd, 10);

	// ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	g_mainWindow = hWnd;

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		_CrtDumpMemoryLeaks();
		exit(0);
		PostQuitMessage(WM_QUIT);
		break;
	case WM_INPUT:
	{
		//Services.Input()->HandleInputEvent(lParam);

	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

