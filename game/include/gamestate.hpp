#ifndef GAMESTATE_HPP

#include <level.hpp>
#include <state.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <random>

class Level;

class GameState : public State
{
public:
	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

private:
	Level currentLevel;
	sf::CircleShape towerRadius;

	Spawner<Enemy> enemySpawner;
	Spawner<Tower> towerSpawner;
	Spawner<Projectile> projectileSpawner;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	sf::Text goldText;
	sf::Sprite cursor;
};

#endif // !GAMESTATE_HPP
