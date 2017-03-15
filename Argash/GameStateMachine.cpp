#include "pch.h"
#include "GameStateMachine.h"

using namespace Tungsten;
static GameStateMachine Tungsten::game;

GameStateMachine::GameStateMachine()
{
}

GameStateMachine::~GameStateMachine()
{
	stateStack.empty();
}

void GameStateMachine::Draw()
{
	stateStack.top()->Draw();
}

void GameStateMachine::Update()
{
	stateStack.top()->Update();
}

void GameStateMachine::Push(IGameState* state)
{
	stateStack.push(state);
}

void GameStateMachine::Pop()
{
	stateStack.pop();
}