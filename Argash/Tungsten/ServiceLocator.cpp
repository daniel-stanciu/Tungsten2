#include "pch.h"
#include "ServiceLocator.h"

using namespace Tungsten;

ServiceLocator Tungsten::Services;
ServiceLocator::ServiceLocator(void)
{
	graphics=NULL;
}


ServiceLocator::~ServiceLocator(void)
{
}

IGraphicsService* ServiceLocator::Graphics()
{
	if(graphics) return graphics;
	return NULL;
}

void ServiceLocator::SetGraphics(IGraphicsService* graphics)
{
	this->graphics=graphics;
}

IConfigurationService* ServiceLocator::Config()
{
	if(config) return config;
	return NULL;
}

void ServiceLocator::SetConfig(IConfigurationService* config)
{
	this->config=config;
}

SpriteTextService* ServiceLocator::SpriteText()
{
	return spriteText;
}

void ServiceLocator::Refresh()
{
	spriteText->DebugClear();
	input->Refresh();
	graphics->Refresh();
}

void ServiceLocator::SetSpriteText(SpriteTextService* spriteText)
{
	this->spriteText=spriteText;
}

InputService* ServiceLocator::Input()
{
	return input;	
}

void ServiceLocator::SetInput(InputService* input)
{
	this->input=input;
}
