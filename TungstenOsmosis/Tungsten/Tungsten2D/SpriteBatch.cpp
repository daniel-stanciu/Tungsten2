
#include "SpriteBatch.h"

using namespace Tungsten;
using namespace DirectX;
SpriteBatch::SpriteBatch(void)
{
}


SpriteBatch::~SpriteBatch(void)
{
}

Sprite* SpriteBatch::MakeSprite(Vector<2,float> upperLeft,Vector<2,float> lowerRight,
								Texture tex,Vector<2,float> upperLeftTexture,Vector<2,float> lowerRightTexture,
								Vector<4,float> tint ,float depth)
{
	//Sprite* ptr=new Sprite((XMFLOAT2)upperLeft,lowerRight,depth,tint);
	return NULL;
}