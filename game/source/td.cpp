#include <td.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <base.hpp>
#include <gamestate.hpp>
#include <menustate.hpp>

Td::Td() : Game(640, 640, "TD")
{
	// Maybe the statemachine is told the relationship.
	gameState = new GameState(stateMachine, assetDatabase, window);
	menuState = new MenuState(stateMachine, gameState);
	gameState->menuState = menuState;
}

Td::~Td()
{
}

void Td::Initialise()
{
	//gameState.SetLevel("assets/level1_metadata.txt");
	stateMachine.SetState(menuState);
	//gameState.Initialise();
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
