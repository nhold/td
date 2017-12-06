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
	Level(AssetDatabase& assetDatabase);
	~Level();

	void Load(std::string filename);
	void Render(sf::RenderWindow& window);

	bool isValid();

	TileMap tileMap;
	BuildingMap buildingMap;
	
	Path* path;
	Base* base;
	std::vector<Wave> waves;
	int startingGold;
	std::string name;
	void Clear();
private:
	AssetDatabase& assetDatabase;

};

#endif // !LEVEL_HPP
