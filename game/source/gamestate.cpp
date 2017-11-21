#include <gamestate.hpp>

void GameState::Initialise()
{
}

void GameState::Shutdown()
{
	enemySpawner.DespawnAll();
	towerSpawner.DespawnAll();
	projectileSpawner.DespawnAll();
}

void GameState::Update()
{
}

void GameState::Render()
{
}

void GameState::ProcessInput(sf::Event currentEvent)
{
}

void GameState::SetLevel(Level* level)
{
	currentLevel = level;
}

