#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tilemap.hpp>
#include <buildingmap.hpp>
#include <vector>
#include <wave.hpp>
#include <assetdatabase.hpp>
#include <enemyspawner.hpp>

class Path;
class Base;
class Enemy;
class Tower;

class Level
{
public:
	Level(EnemySpawner& enemySpawner);
	~Level();

	void Load(std::string tileMapFileName, AssetDatabase& assetDatabase);

	void Update(std::vector<Enemy*>& enemies, std::vector<Tower*>& towers);
	void Render(sf::RenderWindow& window);

	TileMap tileMap;
	BuildingMap buildingMap;
	EnemySpawner& enemySpawner;
	Path* path;
	Base* base;
	std::vector<Wave> waves;
	int currentWave;
	int currentData;
	float time = 0.1f;
	int startingGold;
	int currentGold;
};

#endif // !LEVEL_HPP
