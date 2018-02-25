// Argash.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "Application.h"
#include "ConfigManager.h"
#include "version.h"
#include "Tungsten\D3DRenderer.h"
#include "Tungsten\ServiceLocator.h"
#include "Tungsten\SpriteTextService.h"
#include "GameStateMachine.h"
#include "OBJparser.h"

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
InputService input;
// Forward declarations of functions included in this code module:
WORD				RegisterMainWindow(HINSTANCE hInstance);
BOOL				InitMainWindow(HINSTANCE);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

struct SkyMap
{
	Texture* textures[6];
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	g_currentInstance = hInstance; // Store instance handle in our global variable
	MSG msg;
	Services.SetConfig(new ConfigManager());
	Services.Config()->Load(L"config.cfg");
	input.Initialise(g_mainWindow);
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	_tcscat_s(szTitle, L" ");
	char version[]=VERSION_NUMBER;
	//Abomination of Hell, don't disturb unless Vatar attacks again
	_tcscat_s(szTitle,std::wstring(version,version+strlen(version) ).c_str() );
	LoadString(hInstance, IDC_ARGASH, szWindowClass, MAX_LOADSTRING);
	RegisterMainWindow(hInstance);
	
	// Perform application initialization:
	if (!InitMainWindow (hInstance))
	{
		return FALSE;
	}
	
	Services.SetGraphics(new D3DRenderer(g_mainWindow));
	Services.Graphics()->SetState(SpriteRender);

	Services.SetSpriteText(new SpriteTextService());

	Services.SetInput(new InputService());
	Services.Input()->Initialise(g_mainWindow);
	// Main message loop:
	// Infinite loop
	float customColor[4]={1,0,0,1};
	short wheel=25;
	GameStateMachine machine;
	/*struct TEST
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 tex;
	};
	TEST data[4];
	
	data[0].pos=DirectX::XMFLOAT4(-0.5f,+0.5f,0.4f,1.0f);
	data[1].pos=DirectX::XMFLOAT4(+0.5f,+0.5f,0.4f,1.0f);
	data[2].pos=DirectX::XMFLOAT4(-0.5f,-0.5f,0.4f,1.0f);
	data[3].pos=DirectX::XMFLOAT4(+0.5f,-0.5f,0.4f,1.0f);
										
	data[0].tex=DirectX::XMFLOAT4(0.0f,+1.0f,0.4f,1.0f);
	data[1].tex=DirectX::XMFLOAT4(+1.0f,+1.0f,0.4f,1.0f);
	data[2].tex=DirectX::XMFLOAT4(0.0f,0.0f,0.4f,1.0f);
	data[3].tex=DirectX::XMFLOAT4(+1.0f,0.0f,0.4f,1.0f);
									  
	
	UINT index[6]={0,1,2,2,1,3};   
	MemoryBlock vertex;
	vertex.Ptr=data;
	vertex.Width=sizeof(data);
	MemoryBlock ind;
	ind.Ptr= index;
	ind.Width=sizeof(UINT)*6;*/
	SpriteTextService text;
	SpriteFont font=text.LoadSpriteFont(L"Data\\Fonts\\Arial.bfd",L"Data\\Fonts\\Arial.png");
	Services.Graphics()->SetTexture( Services.Graphics()->LoadTexture(L"Data\\Fonts\\Arial.png") );
	OBJparser obj;
	
	
	

	Camera camera;
	camera.SetLookAt(0.0f,0.0f,0.001f);
	
	camera.SetPosition(0.0f,4.0f,0.0f);
	camera.SetUp(0.0f,1.0f,0.0f);
	MemoryBlock cameraData;
	cameraData.ptr=camera.GetCameraMatrix();
	cameraData.width=16*sizeof(float);
	Vector<2,float> perspective;
	Vector<3,float> move;
	ConfigurationState config;
	PROCESS_MEMORY_COUNTERS_EX memory;
	struct moonlight
	{
		_declspec(align(16)) Vector<3,float> pDirection;
		_declspec(align(16)) Vector<3,float> pDiffuse;
		_declspec(align(16)) Vector<3,float> pAmbient;
	};
	moonlight light;
	light.pDirection=Vector<3,float>(0.0f,-2.0f,-2.5f);
	light.pDiffuse=Vector<3,float>(0.3f,0.1f,0.2f);
	light.pAmbient=Vector<3,float>(0.4f,0.2f,0.3f);
	Timer animationTimer;
	animationTimer.Start();
	GetProcessMemoryInfo(GetCurrentProcess(),(PPROCESS_MEMORY_COUNTERS)&memory,sizeof(memory));
	while (true)     
	{
		GetProcessMemoryInfo(GetCurrentProcess(),(PPROCESS_MEMORY_COUNTERS)&memory,sizeof(memory));
		Services.Refresh();
		config=Services.Config()->Get();
//		input.Reset();
		while( PeekMessage(&msg, NULL, 0, 0,PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		animationTimer.Tick();
		float deltaTime=animationTimer.GetDelta();
		move.x=0;
		move.y=0;
		move.z=0;



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
	Tungsten::ConfigurationState state=Services.Config()->Get();
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARGASH));
	wcex.hCursor		= 0;
	//wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	/*wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+4);*/
	if(state.windowed==state.no)
		wcex.hbrBackground	= (HBRUSH)0;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
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
   HWND hWnd=0;
   Tungsten::ConfigurationState state=Services.Config()->Get();

  
	// Set the window's style.
   if(state.windowed==state.no)
   {
	   //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	   RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	   AdjustWindowRect(&rectangle,WS_POPUP,false);
	 //  HBRUSH brush = (HBRUSH)0;
	  // SetClassLongPtr(g_mainWindow, GCLP_HBRBACKGROUND, (LONG)brush);
	   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
		   200,200,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
   }
   if(state.windowed==state.borderless)
   {
	   //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	   RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	   //AdjustWindowRect(&rectangle,WS_POPUP,false);
	   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP ,
		   0,0,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
   }
	 if(state.windowed==state.yes)
   {
	   //SetWindowLong( hWnd, GWL_STYLE, WS_POPUP );
	   RECT rectangle={0,0,state.resolution.width,state.resolution.height};
	   AdjustWindowRect(&rectangle,WS_POPUP,false);
	   hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION,
		   200,200,rectangle.right,rectangle.bottom,NULL,NULL,g_currentInstance,NULL);
   }
	  if (!hWnd)
   {
      return FALSE;
   }
	// Maximize the window.
	ShowWindow( hWnd,10 );

  // ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   g_mainWindow=hWnd;

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
	switch(message)
    {
		
    case WM_DESTROY:
		_CrtDumpMemoryLeaks();
		exit(0);
        PostQuitMessage(WM_QUIT);
        break;
	case WM_INPUT:
		{
			Services.Input()->HandleInputEvent(lParam);
			
		}
    default:
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

