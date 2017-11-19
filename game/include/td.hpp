#ifndef TD_HPP
#define TD_HPP

#include <game.hpp>
#include <level.hpp>
#include <enemyspawner.hpp>

enum GameState
{
	Menu,
	Game
};

class Td : public Game
{
public:
	Td();
	~Td();

protected:
	void Initialise() override;
	void Update() override;
	void Render() override;
	void ProcessInput(sf::Event currentEvent) override;
	void CreateTypes();

private:
	GameState currentState;
	Level currentLevel;
	sf::CircleShape towerRadius;

	EnemySpawner enemySpawner;
	
	
	std::vector<Tower*> towers;
	std::vector<Tower> towerTypes;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	sf::Text goldText;
	sf::Sprite cursor;

};

#endif
