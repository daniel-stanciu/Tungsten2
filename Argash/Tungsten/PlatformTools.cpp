#include "pch.h"
#include "PlatformTools.h"

using namespace Tungsten;
using namespace Tungsten::PlatformTools;
HRESULT Tungsten::PlatformTools::CreateByteCodeFromFile(WCHAR* filename,MemoryBlock* block)
{
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

HRESULT Tungsten::PlatformTools::CreateStringBufferFromFile(WCHAR* filename, _Out_ std::stringstream** p_stringBuff)
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

 HANDLE Tungsten::PlatformTools::CreateFileHandle(WCHAR* filename,int desiredAcces)
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
	
		/*
	using namespace Windows::Storage;
	//Get the path where the package was installed
	auto folder=Windows::ApplicationModel::Package::Current->InstalledLocation;
	//Cast native string into COM object
	auto ref_name=ref new Platform::String(filename);
	//Create the full path by concatenating the folder path and the relative path
	ref_name=Platform::String::Concat(L"\\",ref_name);
	ref_name=Platform::String::Concat(folder->Path,ref_name);

	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {0};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;
	Microsoft::WRL::Wrappers::FileHandle file(CreateFile2(
		ref_name->Data(),
		GENERIC_READ,
		FILE_SHARE_READ,
		OPEN_EXISTING,
		&extendedParams));
	if(file.Get()==INVALID_HANDLE_VALUE)
	{
		throw ref new Platform::FailureException();
	}
	//64 bit integer union
	FILE_STANDARD_INFO fileInfo = {0};
	if (!GetFileInformationByHandleEx(
		file.Get(),
		FileStandardInfo,
		&fileInfo,
		sizeof(fileInfo)
		))
	{
		throw ref new Platform::FailureException();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw ref new Platform::OutOfMemoryException();
	}
	*/
}

HRESULT Tungsten::PlatformTools::DecodePixelsFromFile(WCHAR* filename,MemoryBlock* block)
{
	//TODO : Make more general


	void* fileData;
	int* bufferWidth;
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