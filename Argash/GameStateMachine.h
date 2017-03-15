#pragma once
#include "pch.h"
#include "IGameState.h"
#include <stack>
namespace Tungsten
{
class GameStateMachine
{
public:
	GameStateMachine();
	~GameStateMachine();
	void Push(IGameState* state);
	void Pop();
	void Update();
	void Draw();

private:
	std::stack<IGameState*> stateStack;
};

extern GameStateMachine game;
}