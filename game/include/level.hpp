#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tilemap.hpp>
#include <buildingmap.hpp>
#include <vector>
#include <wave.hpp>
#include <assetdatabase.hpp>

class Path;
class Base;
class Enemy;
class Tower;

class Level
{
public:
	Level();
	~Level();

	void Load(std::string tileMapFileName, AssetDatabase& assetDatabase);

	void Update(std::vector<Enemy*>& enemies, std::vector<Tower*>& towers);
	void Render(sf::RenderWindow& window);

	TileMap tileMap;
	BuildingMap buildingMap;
	Path* path;
	Base* base;
	std::vector<Wave> waves;
	int startingGold;
	int currentGold;
};

#endif // !LEVEL_HPP
