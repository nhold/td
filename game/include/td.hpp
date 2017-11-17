#ifndef TD_HPP
#define TD_HPP

#include <game.hpp>

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
	int gold;
	TileMap tileMap;
	BuildingMap buildingMap;
	sf::CircleShape towerRadius;
	std::vector<Enemy*> enemies;
	std::vector<Enemy> enemyTypes;

	std::vector<Tower*> towers;
	std::vector<Tower> towerTypes;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
	Path* path;
	Base* base;
	sf::Text goldText;
	sf::Sprite cursor;


};

#endif
