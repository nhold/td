#ifndef BUILDINGMAP_HPP
#define BUILDINGMAP_HPP

#include <tilemap.hpp>

class BuildingMap
{
public:
	BuildingMap();
	bool isBlocked[TileMap::tileMapSize][TileMap::tileMapSize];
	void Clear();
};

#endif // !BULIDINGMAP_HPP
