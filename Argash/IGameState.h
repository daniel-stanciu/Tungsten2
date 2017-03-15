#pragma once
#include "pch.h"
namespace Tungsten
{
class GameStateMachine;
class IGameState
{
public:
	virtual void Update()=0;
	virtual void Draw()=0;
private:
	
};


}