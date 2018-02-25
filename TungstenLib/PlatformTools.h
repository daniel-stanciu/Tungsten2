#pragma once
#include "pch.h"
#include <sstream>
#include "DataContainers.h"


extern ::HWND g_mainWindow;
namespace Tungsten
{
	namespace PlatformTools
	{
		
		//Get content of file as array of bytes
		HRESULT CreateByteCodeFromFile(const WCHAR* filename,MemoryBlock* block);
		//Get content of file as string buffer
		HRESULT CreateStringBufferFromFile(const WCHAR* filename,::std::stringstream** p_stringBuff);

		HRESULT DecodePixelsFromFile(const WCHAR* filename,MemoryBlock* block);

		HANDLE CreateFileHandle(const WCHAR* filename,int desiredAcces);
		inline void PostError(WCHAR* message=NULL)
		{
			DWORD error=GetLastError();
			if(!error)
			{
				MessageBox(g_mainWindow,L"Undefined Error",L"Error",MB_OK);
				return;
			}
			WCHAR errorstring[100];
			DWORD errormessage=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL,error,
			NULL,errorstring,
			100,NULL);
			if(message)
			{
				MessageBox(g_mainWindow,message,L"Fatal Error",MB_OK);
				return;
			}
			MessageBox(g_mainWindow,errorstring,L"Fatal Error",MB_OK);
			exit(0);
		}
		 enum FileAccess
		 {	
			 Create	=	0x00000001L,
			 Read	=	0x80000000L,
			 Write	=	0x40000000L
		 };
		inline bool Error(HRESULT result)
		{
			if ( FAILED(result) ) return true;
			return false;
		}
		inline bool Error(BOOL result)
		{
			if(!result) return true;
			return false;
		}
		inline void CheckFatalError(HRESULT result)
		{
			if ( FAILED(result) ) PostError();
		}
		inline void CheckFatalError(BOOL result)
		{
			if(!result) PostError();
		}

		//Platform::Array<byte>^ GetBytecodeFromFile(std::wstring filename);
	}
}