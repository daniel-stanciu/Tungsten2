#pragma once
#include "pch.h"
#include "IGraphicsService.h"
#include "IConfigurationService.h"
#include "SpriteTextService.h"
#include "InputService.h"
namespace Tungsten
{

class ServiceLocator
{
private:
	IGraphicsService* graphics;
	IConfigurationService* config;
	SpriteTextService* spriteText;
	InputService* input;
public:
	void SetGraphics(IGraphicsService*);
	void SetConfig(IConfigurationService*);
	void SetSpriteText(SpriteTextService*);
	void SetInput(InputService*);
	IGraphicsService* Graphics();
	IConfigurationService* Config();
	SpriteTextService* SpriteText();
	InputService* Input();
	//Refresch is called in order to update Input and other services for a new frame
	void Refresh();
	ServiceLocator(void);
	~ServiceLocator(void);
};
extern ServiceLocator Services;

}
