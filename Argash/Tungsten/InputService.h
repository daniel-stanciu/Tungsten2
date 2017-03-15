#pragma once
#include "pch.h"
#include "DataContainers.h"

enum MouseButton
{
	LButton = 0,
	RButton = 1,
	MButton = 2,
				
	//Left Button	
	Button1 = 0,
	//Right Button	
	Button2 = 1,
	//Middle Button	
	Button3 = 2,
				
	//XButton 1		
	Button4 = 3,
	//XButton 2		
	Button5 = 4
};		
				
class InputService
{
private:
	int index;
	
	
public:
	class tag_keyboard
	{
	private:
		UINT keys[256];
	public:
		void Push(UINT vKey);
		void Pop(UINT vKey);
		bool IsPressed(UINT vKey);
		friend class InputService;
	} keyboard;
	class tag_mouse
	{
	private:
		SHORT wheelDelta;
		USHORT buttonFlags; 
		Tungsten::Vector<2,LONG> position;
		Tungsten::Vector<2,LONG> movement;
	public:
		void SetFlags(USHORT mouseButtonFlags);
		bool IsPressed(MouseButton button);
		void SetCursor(LONG x,LONG y);
		void SetPosition(LONG x,LONG y);
		void SetMovement(LONG x,LONG y);
		Tungsten::Vector<2> GetMouseCoords();
		Tungsten::Vector<2> GetMouseMovement();
		short GetWheelDelta();
		void SetWheelDelta(SHORT wheelDelta);
		friend class InputService;
		
	} mouse;
	RAWINPUTDEVICE inputDevices[2];
	InputService();
	~InputService(void);
	void Initialise(HWND windowHandle);
	//Pass the lParam of the WM_INPUT event for processing
	void Refresh();
	void HandleInputEvent(LPARAM lParam);
};

