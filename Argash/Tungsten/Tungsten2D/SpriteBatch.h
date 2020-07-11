#pragma once

#include "ServiceLocator.h"
#include "Sprite.h"
#include "DataContainers.h"
namespace Tungsten
{

class SpriteBatch
{
private:
	Dictionary<Texture,MemoryCache<Sprite>> cache;
	
public:
	Texture GetTexture(std::wstring path);
	void Draw(Sprite* sprite);
	Sprite* MakeSprite(
		Vector<2,float> upperLeft,Vector<2,float> lowerRight,
		Texture tex,Vector<2,float> upperLeftTexture,Vector<2,float> lowerRightTexture,
		Vector<4,float> tint =Vector<4,float>(1.0f,1.0f,1.0f,1.0f),float depth=1
		);
	SpriteBatch(void);
	~SpriteBatch(void);
};

}