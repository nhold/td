#ifndef TD_HPP
#define TD_HPP

#include <game.hpp>
#include <level.hpp>
#include <enemyspawner.hpp>
#include <projectile.hpp>

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

#endif
