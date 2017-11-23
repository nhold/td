#ifndef MENUSTATE_HPP

#include <state.hpp>
#include <statemachine.hpp>

class GameState;

class MenuState : public State
{
public:
	MenuState(StateMachine& stateMachine, GameState* gameState);
	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

private:
	GameState* gameState;
	StateMachine& stateMachine;
};

#endif // !MENUSTATE_HPP

