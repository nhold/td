#include <td.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <base.hpp>

Td::Td() : Game(640, 640, "TD"), gameState(assetDatabase, window)
{

}

Td::~Td()
{
}

void Td::Initialise()
{
	gameState.SetLevel("assets/level1_metadata.txt");
	gameState.Initialise();
}

void Td::Update()
{
	Game::Update();
	gameState.Update();
}


void Td::Render()
{
	gameState.Render();
}

void Td::ProcessInput(sf::Event currentEvent)
{
	gameState.ProcessInput(currentEvent);
}