#include <td.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <base.hpp>
#include <gamestate.hpp>
#include <menustate.hpp>

Td::Td() : Game(640, 640, "TD")
{
	// TODO: Maybe the statemachine is told the relationship.
	gameState = new GameState(stateMachine, assetDatabase, window);
	menuState = new MenuState(stateMachine, assetDatabase, window);
	gameState->menuState = menuState;
	menuState->gameState = gameState;
}

Td::~Td()
{
}

void Td::Initialise()
{
	stateMachine.SetState(menuState);
}

void Td::Update()
{
	Game::Update();
	stateMachine.currentState->Update();
}


void Td::Render()
{
	stateMachine.currentState->Render();
}

void Td::ProcessInput(sf::Event currentEvent)
{
	stateMachine.currentState->ProcessInput(currentEvent);
}
