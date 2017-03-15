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
	
	

	Tungsten::Texture planeTex=Services.Graphics()->LoadTexture(L"Data\\Textures\\A6M_ZERO_D.png");
	//obj.Parse(L"Data\\Models\\broken.obj",L"Data\\Models\\broken.bmd",-1);
	Model broken=obj.Load(L"Data\\Models\\broken.bmd");
	broken.worldMatrix=XMMatrixTranslation(0.0f,0.0f,0.0f);

	//obj.Parse(L"Data\\Models\\plane.obj",L"Data\\Models\\plane.bmd",-1);
	Model plane=obj.Load(L"Data\\Models\\plane.bmd");
	plane.worldMatrix=XMMatrixRotationZ(-1.2)*XMMatrixTranslation(20.0f,2.0f,-50.0f);
	
	Tungsten::Texture terrainTex=Services.Graphics()->LoadTexture(L"Data\\Textures\\grass.jpg");
	//obj.Parse(L"Data\\Models\\terrain.obj",L"Data\\Models\\terrain.bmd");
	Model terrain=obj.Load(L"Data\\Models\\terrain.bmd");
	terrain.worldMatrix=XMMatrixScaling(1000.0f,1000.0f,1000.0f)*XMMatrixTranslation(0.0f,0.0f,0.0f);

	Tungsten::Texture soldierTex=Services.Graphics()->LoadTexture(L"Data\\Textures\\soldier.png");
	//obj.Parse(L"Data\\Models\\soldier.obj",L"Data\\Models\\soldier.bmd",-1);
	Model soldier=obj.Load(L"Data\\Models\\soldier.bmd");
	soldier.worldMatrix=XMMatrixScaling(0.6f,0.6f,0.6f)*XMMatrixTranslation(15.0f,0.0f,-20.0f);
	
	Tungsten::Texture heliTex=Services.Graphics()->LoadTexture(L"Data\\Textures\\heli_base.png");
	//obj.Parse(L"Data\\Models\\heli_base.obj",L"Data\\Models\\heli_base.bmd",-1);
	Model heli_base=obj.Load(L"Data\\Models\\heli_base.bmd");
	heli_base.worldMatrix=XMMatrixIdentity();

	Tungsten::Texture rotorTex=Services.Graphics()->LoadTexture(L"Data\\Textures\\rotor.jpg");
	//obj.Parse(L"Data\\Models\\rotor.obj",L"Data\\Models\\rotor.bmd",-1);
	Model rotor=obj.Load(L"Data\\Models\\rotor.bmd");
	rotor.worldMatrix=XMMatrixIdentity();

	//obj.Parse(L"Data\\Models\\rear_rotor.obj",L"Data\\Models\\rear_rotor.bmd",-1);
	Model rear_rotor=obj.Load(L"Data\\Models\\rear_rotor.bmd");
	rear_rotor.worldMatrix=XMMatrixIdentity();
	
 	//obj.Parse(L"Data\\Models\\cube.obj",L"Data\\Models\\cube.bmd",-1);
	Model cube=obj.Load(L"Data\\Models\\cube.bmd");
	cube.worldMatrix=XMMatrixIdentity();
	Texture sky=Services.Graphics()->LoadSkyMap();

	//obj.Parse(L"Data\\Models\\billboard.obj",L"Data\\Models\\billboard.bmd",-1);
	Model billboard=obj.Load(L"Data\\Models\\billboard.bmd");
	cube.worldMatrix=XMMatrixIdentity();
	

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
		
		if(Services.Input()->keyboard.IsPressed(VK_UP))
		{
			move.z=0.11f;
		}
		if(Services.Input()->keyboard.IsPressed(VK_DOWN))
		{
			move.z=-0.11f;
		}
		if(Services.Input()->keyboard.IsPressed(VK_RIGHT))
		{
			move.x=0.11f;
		}
		if(Services.Input()->keyboard.IsPressed(VK_LEFT))
		{
			move.x=-0.11f;
		}

		perspective=Services.Input()->mouse.GetMouseMovement();
		perspective.x/=config.resolution.width;
		perspective.y/=config.resolution.height;
		camera.SetPerspectiveDelta(perspective.x,perspective.y);
		
		camera.MoveRelative(move.x,move.y,move.z);
		
		camera.GetCameraMatrix();
		
		Services.Graphics()->SetState(SimpleModel);
		
		Services.Graphics()->SetConstantBufferVS(cameraData,0);
		Services.Graphics()->SetConstantBufferPS(MemoryBlock(&light,sizeof(light)),0);

		Services.Graphics()->SetTexture(terrainTex);
		obj.Draw(terrain);

		Services.Graphics()->SetTexture(planeTex);
		obj.Draw(plane);
		obj.Draw(broken);

		Services.Graphics()->SetTexture(soldierTex);
		obj.Draw(soldier);

		Services.Graphics()->SetTexture(heliTex);
		obj.Draw(heli_base);
		heli_base.worldMatrix=XMMatrixScaling(1.7f,1.7f,1.7f)*XMMatrixTranslation(50.0f,0.0f,50.0f);

		Services.Graphics()->SetTexture(rotorTex);
		rotor.worldMatrix=XMMatrixScaling(1.7f,1.7f,1.7f)*XMMatrixRotationY(animationTimer.GetTotalFloat()*5)*XMMatrixTranslation(50.0f,0.0f,50.0f);
		obj.Draw(rotor);
		rear_rotor.worldMatrix=XMMatrixScaling(1.7f,1.7f,1.7f)*XMMatrixRotationX(animationTimer.GetTotalFloat() * 5)* XMMatrixTranslation(50.0f,0.0f,50.0f);
		obj.Draw(rear_rotor);

		

		Services.Graphics()->SetState(SkyBox);

		Vector<3,float> position=camera.GetPosition();

		Services.Graphics()->SetTexture(sky);
		cube.worldMatrix=XMMatrixScaling(500.0f,500.0f,500.0f)
			*XMMatrixTranslation(position.x,position.y,position.z)
			* *camera.GetCameraMatrix();
		Vector<4,float> tint(1.0f,1.0f,1.0f,1.0f);
		Services.Graphics()->SetConstantBufferPS(MemoryBlock(&tint,sizeof(tint)),0);
		Services.Graphics()->SetConstantBufferVS(MemoryBlock(&cube.worldMatrix,sizeof(XMMATRIX)),0);
		Services.Graphics()->SetBuffers(cube.vertex,cube.index);
		Services.Graphics()->DrawIndexed(cube.index.width/sizeof(UINT32),0,0);

		Services.Graphics()->SetState(Grass);

		billboard.worldMatrix=XMMatrixScaling(10.0f,10.0f,10.0f);
		Services.Graphics()->SetTexture(terrainTex);
		Services.Graphics()->SetConstantBufferVS(cameraData,0);
		Services.Graphics()->SetConstantBufferVS(MemoryBlock(&billboard.worldMatrix,sizeof(XMMATRIX)),1);

		Services.Graphics()->SetConstantBufferPS(MemoryBlock(&light,sizeof(light)),0);
		
		Vector<3,float> instance[100];
		for(int i=0;i<100;i++)
		{
			instance[i].x=i/10.0f;
			instance[i].y=i/10.0f;
			instance[i].z=i/10.0f;
		}
		

		Services.Graphics()->DrawInstanced(billboard.vertex,billboard.index,MemoryBlock(instance,sizeof(float)*300));


		Services.Graphics()->SetState(SpriteRender);
		
		char string[100];
		sprintf_s(string,100,"Perspective:{%.2f , %.2f}\nMemory:%d MB\nPosition: %.2f %.2f %.2f\n ms:%.2f",perspective.x,perspective.y,
			memory.PrivateUsage/(1024*1024),
			position.x,position.y,position.z,
			animationTimer.GetDelta()
			);
		Services.SpriteText()->DebugWrite(string);
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

