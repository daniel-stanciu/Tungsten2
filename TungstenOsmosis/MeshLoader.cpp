#include "pch.h"
#include "MeshLoader.h"

#include < vcclr.h >

void TungstenOsmosis::MeshLoader::Parse(System::String ^ path, System::String ^ outputPath)
{
	pin_ptr<const wchar_t> wch_in = PtrToStringChars(path);
	pin_ptr<const wchar_t> wch_out = PtrToStringChars(outputPath);
	_impl->Parse(wch_in, wch_out);
}

TungstenOsmosis::Mesh^ TungstenOsmosis::MeshLoader::Load(System::String ^ path)
{
	pin_ptr<const wchar_t> wch = PtrToStringChars(path);
	

	return gcnew Mesh(_impl->Load(wch));
}

TungstenOsmosis::MeshLoader::MeshLoader()
{
	_impl = new Tungsten::OBJparser();
}

