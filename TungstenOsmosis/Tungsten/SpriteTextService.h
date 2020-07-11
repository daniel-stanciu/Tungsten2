#pragma once

#include "IGraphicsService.h"
#include "PlatformTools.h"
#include "DataContainers.h"
#include "Sprite.h"
namespace Tungstenss
{
struct SpriteChar
{
	int x;
	int y;
	int width;
	int height;
};

struct tag_SpriteFont
{
	SpriteChar	charData[256];
	Texture		texture;
};
typedef tag_SpriteFont* SpriteFont;
class SpriteTextService
{
private:
	//For debug messages
	Vector<2,float> cursorPos;
	SpriteFont	debugFont;

	void MakeCharSprite(Vector<2,float> cursor,float size,SpriteChar charData,Sprite* out);
public:
	SpriteTextService();
	~SpriteTextService();
	//Font Size is 1->100(fullscreen)
	//Allignment refers to an offset from the side of the screen.
	void	DrawString(Vector<2,float>& cursor,float size,SpriteFont font,char* string,float allign=0.0f);
	void	DrawStringCentered(Vector<2,float> center,float size,SpriteFont font,char* string);
	static SpriteFont LoadSpriteFont(const WCHAR* binaryPath,const WCHAR* texturePath);
	void DebugWrite(char* text);
	void DebugClear();
};


}