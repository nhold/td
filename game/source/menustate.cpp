#include <menustate.hpp>
#include <gamestate.hpp>

MenuState::MenuState(StateMachine& stateMachine, GameState * gameState) : stateMachine(stateMachine)
{
	this->gameState = gameState;
}

void MenuState::Initialise()
{
}

void MenuState::Shutdown()
{
}

void MenuState::Update()
{
}

void MenuState::Render()
{
}

void MenuState::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::KeyReleased)
	{
		if (currentEvent.key.code == sf::Keyboard::Num1)
		{
			// Tell the game state what level it will be playing.
			gameState->SetLevel("assets/level1_metadata.txt");
			stateMachine.SetState(gameState);
		}
	}
}
