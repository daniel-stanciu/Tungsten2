#pragma once
#include "IGameState.h"
#include "GameStateMachine.h"
#include "Tungsten\ServiceLocator.h"
class StateMainMenu :
	public Tungsten::IGameState
{
private:

public:
	virtual void Draw();
	virtual void Update();
	StateMainMenu(void);
	~StateMainMenu(void);
};

