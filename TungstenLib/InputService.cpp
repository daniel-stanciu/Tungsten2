#include "pch.h"
#include "InputService.h"
#include "PlatformTools.h"
using namespace DirectX;

InputService::InputService()
{

}

void InputService::Initialise(HWND windowHandle)
{
	inputDevices[0].hwndTarget = windowHandle;
	inputDevices[1].hwndTarget = windowHandle;
	inputDevices[0].usUsagePage = 0x01; // Generic desktop page
	inputDevices[0].usUsage = 0x06;     // Keyboard
	inputDevices[0].dwFlags = 0;
	inputDevices[1].usUsagePage = 0x01; // Generic desktop page
	inputDevices[1].usUsage = 0x02;     // Mouse
	inputDevices[1].dwFlags = 0;
	using namespace Tungsten::PlatformTools;
	if( Error(RegisterRawInputDevices(inputDevices, 2, sizeof(RAWINPUTDEVICE))) )
		PostError();
	for(int i=0;i<256;i++)
	 keyboard.Pop(i);
	POINT pt;
	pt.x=0;
	pt.y=0;
	ClientToScreen(NULL,&pt);
	mouse.SetCursor(pt.x,pt.y);
	mouse.position.x=0;
	mouse.position.y=0;
	
}

InputService::~InputService(void)
{
	Refresh();
}

void InputService::Refresh()
{
	mouse.wheelDelta = 0;
	mouse.movement.x = 0;
	mouse.movement.y = 0;
	//Hide the cursor
	while(ShowCursor(0)>-1);
}

void InputService::tag_keyboard::Push(UINT vKey)
{
	if(vKey<256)
	keys[vKey]=0x1;
	
}

void InputService::tag_keyboard::Pop(UINT vKey)
{
	if(vKey<256)
	keys[vKey]=0x0;
	
}
bool InputService::tag_keyboard::IsPressed(UINT vKey)
{
	if(vKey<256)
	return keys[vKey]!=0;
	return false;
}

Tungsten::Vector<2> InputService::tag_mouse::GetMouseCoords()
{
	return position;
}

Tungsten::Vector<2> InputService::tag_mouse::GetMouseMovement()
{
	return movement;
}

bool InputService::tag_mouse::IsPressed(MouseButton button)
{
	return ( this->buttonFlags&(1<<button) )!=0;
}

void InputService::tag_mouse::SetFlags(USHORT mouseButtonFlags)
{
	buttonFlags=mouseButtonFlags;
}

void InputService::tag_mouse::SetMovement(LONG x,LONG y)
{
	movement.x=x;
	movement.y=y;
	position.x+=x;
	position.y+=y;
}

void InputService::tag_mouse::SetPosition(LONG x,LONG y)
{
	movement.x=x-position.x;
	movement.y=y-position.y;
	position.x=x;
	position.y=y;
}

void InputService::tag_mouse::SetWheelDelta(SHORT wheelDelta)
{
	this->wheelDelta=wheelDelta / WHEEL_DELTA;
	//WHEEL_DELTA is 120,representing one dent in the scroll wheel
}

short InputService::tag_mouse::GetWheelDelta()
{
	return wheelDelta;
}

void InputService::HandleInputEvent(LPARAM lParam)
{
	UINT size;
	GetRawInputData( (HRAWINPUT) lParam,RID_INPUT,NULL,&size,sizeof(RAWINPUTHEADER));
	BYTE* bytes=new BYTE[size];
	GetRawInputData( (HRAWINPUT) lParam,RID_INPUT,bytes,&size,sizeof(RAWINPUTHEADER));
	RAWINPUT* raw=(RAWINPUT*) bytes;
	if( raw->header.dwType==RIM_TYPEKEYBOARD )
		{	
			if (raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN)
			{
				
				this->keyboard.Push(raw->data.keyboard.VKey);
			}
			if(raw->data.keyboard.Flags & RI_KEY_BREAK) 
				this->keyboard.Pop(raw->data.keyboard.VKey);
			
		}
	if( raw->header.dwType==RIM_TYPEMOUSE )
	{
		if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			this->mouse.SetPosition(raw->data.mouse.lLastX,raw->data.mouse.lLastY);
		}
		else
		{
			this->mouse.SetMovement(raw->data.mouse.lLastX,raw->data.mouse.lLastY);
		}
		this->mouse.SetFlags(raw->data.mouse.usButtonFlags);
		if(raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			this->mouse.SetWheelDelta(raw->data.mouse.usButtonData);
		}
	
	}
	delete[] bytes;
}
void InputService::tag_mouse::SetCursor(LONG x,LONG y)
{
	this->position.x=x;
	this->position.y=y;
}