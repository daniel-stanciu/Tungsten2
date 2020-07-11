
#include "PlatformTools.h"
#include "D3DRenderer.h"

using namespace Tungsten;
using namespace Tungsten::PlatformTools;
HRESULT Tungsten::PlatformTools::CreateByteCodeFromFile(const WCHAR* filename,MemoryBlock* block)
{
	WCHAR directory[1000];
	GetCurrentDirectoryW(1000,(LPWSTR) &directory);
	HANDLE file;
	file=CreateFileHandle(filename,FileAccess::Read);
	_FILE_STANDARD_INFO info;
	CheckFatalError( GetFileInformationByHandleEx(file,FileStandardInfo,&info,sizeof(info)) );
	
	block->width=info.EndOfFile.LowPart;
	
	block->ptr=new byte[block->width+1];
	if(!ReadFile(file,block->ptr,block->width,NULL,NULL))
#ifdef _DEBUG
	{
	const DWORD size=100+1;
	DWORD error=GetLastError();
	WCHAR errorstring[size];
	DWORD errormessage=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,error,
		NULL,errorstring,
		size,NULL);
	CloseHandle(file);
	free(block->ptr);
	block->ptr=NULL;
	return E_FAIL;
	}
#else
	{
		CloseHandle(file);
		free(*p_buffer);
		*p_buffer=NULL;
		return E_FAIL;
	}
#endif
	
	CloseHandle(file);
	return S_OK;
}

HRESULT Tungsten::PlatformTools::CreateStringBufferFromFile(const WCHAR* filename, _Out_ std::stringstream** p_stringBuff)
{

	HANDLE file;
	*p_stringBuff=NULL;
	file=CreateFileHandle(filename,Read);
	if(file==NULL) return E_INVALIDARG;
	char* array_buffer;
	/*
	BOOL WINAPI GetFileInformationByHandleEx(
  _In_   HANDLE hFile,
  _In_   FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
  _Out_  LPVOID lpFileInformation,
  _In_   DWORD dwBufferSize
	);
	*/
	_FILE_STANDARD_INFO info;
	GetFileInformationByHandleEx(file,FileStandardInfo,&info,sizeof(info));

	DWORD size=info.EndOfFile.LowPart;
	DWORD read_bytes;
	array_buffer=(char*)malloc(size+1);
	if(!ReadFile(file,array_buffer,size,&read_bytes,NULL))
#ifdef _DEBUG
	{
	const DWORD size=100+1;
	DWORD error=GetLastError();
	WCHAR errorstring[size];
	DWORD errormessage=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,error,
		NULL,errorstring,
		size,NULL);
	CloseHandle(file);
	free(array_buffer);
	return E_FAIL;
	}
#else
	{
		CloseHandle(file);
		return E_FAIL;
	}
#endif
	array_buffer[size]=0;
	*p_stringBuff=new std::stringstream(array_buffer);
	CloseHandle(file);
	return S_OK;
}

 HANDLE Tungsten::PlatformTools::CreateFileHandle(const WCHAR* filename,int desiredAcces)
{
	
	//Win32 Implementation
	std::wstring filepath;
	bool create=desiredAcces&Create;
	desiredAcces=desiredAcces&~Create;

#ifdef WIN32
	
	WCHAR buffer[260];
	GetModuleFileName(NULL,buffer,260);
	//Remove the trailing executable file name
	int index=0;
	int pathLength;
	while(index<260)
	{
		if(buffer[index]=='\\') pathLength=index;
		index++;
	}
	filepath.append(buffer,0,pathLength);
#endif
#ifdef WINRT
	filepath=Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data();
#endif
	filepath.append(L"\\");
	filepath.append(filename);
	//HANDLE file=CreateFile2(filepath.c_str(),GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING,NULL);
	int flag;
	if(create) flag=CREATE_ALWAYS;
	else flag=OPEN_EXISTING;
	HANDLE file=CreateFile2(filepath.c_str(),desiredAcces,NULL,flag,NULL);
	if(file==INVALID_HANDLE_VALUE)
#ifdef _DEBUG
	{
		DWORD error=GetLastError();
		const DWORD size=500+1;
		WCHAR errorstring[size];
		DWORD errormessage=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL,error,
			NULL,errorstring,
			size,NULL);
		wcscat_s(errorstring,L"\n");
		wcscat_s(errorstring,filepath.c_str());
		Tungsten::PlatformTools::PostError(errorstring);
		return NULL;
		
	}
#else
	{
		*file_handle=NULL;
		DWORD error=GetLastError();
		const DWORD size=100+1;
		WCHAR errorstring[size];
		DWORD errormessage=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL,error,
			NULL,errorstring,
			size,NULL);
		*file_handle=NULL;
		return E_INVALIDARG;
		return E_INVALIDARG;
	}
#endif
	return file;
	
}

//IGraphicsService * Tungsten::PlatformTools::RendererForHwnd(HWND window, ConfigurationState config)
//{
//	UINT flags = 0;
//	flags |= D3D11_CREATE_DEVICE_SINGLETHREADED; //D3D calls will only come from one thread
//	flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;	//required for D2D interop
//#if defined _DEBUG
//	flags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//	DXGI_SWAP_CHAIN_DESC1 scDescription;
//	//scDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	//scDescription.BufferDesc.Height = config.resolution.height;
//	//scDescription.BufferDesc.Width = config.resolution.width;
//	//DXGI_RATIONAL refreshRate = { 0,0 };	//Use active refresh rate
//	//scDescription.BufferDesc.RefreshRate = refreshRate;
//	////Default values, black magic inside
//	//scDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//	//scDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//
//	//scDescription.BufferCount = 2;
//	//scDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	//scDescription.Flags = 0;
//	//scDescription.OutputWindow = window;
//	//scDescription.SampleDesc.Count = 1;
//	//scDescription.SampleDesc.Quality = 0;
//	//scDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	//scDescription.Windowed = true;
//	//if (config.windowed == config.no)
//	//	scDescription.Windowed = false;
//	scDescription.Width = 0;
//	scDescription.Height = 0;
//	scDescription.BufferCount = 2;
//	scDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	scDescription.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
//	scDescription.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
//	scDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	scDescription.SampleDesc.Count = 1;
//	scDescription.SampleDesc.Quality = 0;
//	scDescription.Scaling = DXGI_SCALING_NONE;
//	scDescription.Stereo = false;
//	scDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
//
//	ID3D11Device* device;
//	IDXGISwapChain1* swapChain;
//	ID3D11DeviceContext* context;
//	D3D11CreateDevice(
//		NULL,						//Use default adapter
//		D3D_DRIVER_TYPE_HARDWARE,	//Use hardware renderer
//		NULL,						//Sofware DLL, not used
//		flags,
//		NULL,						//Uses default feature level array D3D_FEATURE_LEVEL_11_0 to 9_1
//		NULL,						//Number of feature levels, use default
//		D3D11_SDK_VERSION,
//		&device,
//		NULL,
//		&context
//	);
//	IDXGIDevice * DXGIDevice;
//	device->QueryInterface(__uuidof(IDXGIDevice), (void **)&DXGIDevice);
//
//	IDXGIAdapter * DXGIAdapter;
//	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&DXGIAdapter);
//
//	IDXGIFactory2 * IDXGIFactory;
//	DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&IDXGIFactory);
//
//	IDXGIFactory->CreateSwapChainForHwnd(device, window, &scDescription, nullptr, nullptr, &swapChain);
//
//	//The context will be recreated by the renderer
//	auto renderer = new D3DRenderer(device, swapChain);
//	context->Release();
//	return renderer;
//}

HRESULT Tungsten::PlatformTools::DecodePixelsFromFile(const WCHAR* filename,MemoryBlock* block)
{
	//TODO : Make more general


	//void* fileData;
	//int* bufferWidth;
	/*
	CreateByteCodeFromFile(filename,&fileData,bufferWidth);

	*/
	/*IWICImagingFactory2* factory;
	CheckFatalError(
	CoCreateInstance(CLSID_WICImagingFactory2,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory))
	);
	
	IStream* stream=SHCreateMemStream((BYTE*)fileData,*bufferWidth);
	IWICBitmapDecoder* decoder;

	CheckFatalError(
	factory->CreateDecoderFromStream(stream,NULL,WICDecodeMetadataCacheOnDemand,&decoder)
	);

	IWICBitmapFrameDecode* frame;
	decoder->GetFrame(0,&frame);*/
	return S_OK;

}