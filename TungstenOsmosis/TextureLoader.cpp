#include "pch.h"
#include "TextureLoader.h"

#include < vcclr.h >

TungstenOsmosis::TextureLoader::TextureLoader()
{
	
	_impl = new Tungsten::TextureLoader();

}

TungstenOsmosis::Texture ^ TungstenOsmosis::TextureLoader::Get(System::String ^ arg)
{
	pin_ptr<const wchar_t> wch = PtrToStringChars(arg);
	
	return gcnew Texture(_impl->Get(wch));
}
