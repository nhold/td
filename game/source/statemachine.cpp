#include <statemachine.hpp>
#include <state.hpp>

StateMachine::StateMachine()
{
	currentState = nullptr;
}

StateMachine::~StateMachine()
{
}

void StateMachine::SetState(State* newState)
{
	if (currentState != nullptr)
	{
		currentState->Shutdown();
	}

	newState->Initialise();
	currentState = newState;
}

