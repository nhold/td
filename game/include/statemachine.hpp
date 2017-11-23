#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

class State;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void SetState(State* newState);
	State* currentState;
};

#endif // !STATEMACHINE_HPP
