#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tilemap.hpp>
#include <buildingmap.hpp>
#include <vector>
#include <wave.hpp>
#include <assetdatabase.hpp>
#include <enemyspawner.hpp>
#include <projectile.hpp>

class Path;
class Base;
class Enemy;
class Tower;

class Level
{
public:
	Level(Spawner<Enemy>& enemySpawner, Spawner<Tower>& towerSpawner, Spawner<Projectile>& projectileSpawner);
	~Level();

	void Load(std::string tileMapFileName, AssetDatabase& assetDatabase);

	void Update();
	
	void Render(sf::RenderWindow& window);

	TileMap tileMap;
	BuildingMap buildingMap;
	
	Path* path;
	Base* base;
	std::vector<Wave> waves;
	int currentWave;
	int currentData;
	float time = 0.1f;
	int startingGold;
	int currentGold;

private:

	Spawner<Enemy>& enemySpawner;
	Spawner<Tower>& towerSpawner;
	Spawner<Projectile>& projectileSpawner;

	std::vector<Enemy*> deadEnemyVector;
	std::vector<Projectile*> deadProjectileVector;

	void UpdateWave();
	void UpdateEnemies();
	void UpdateTowers();
	void PostUpdate();
};

#endif // !LEVEL_HPP
