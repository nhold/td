#ifndef BULIDINGMAP_HPP
#define BUILDINGMAP_HPP

#include <tilemap.hpp>

class BuildingMap
{
public:
	BuildingMap();
	bool isBlocked[TileMap::tileMapSize][TileMap::tileMapSize];

};

#endif // !BULIDINGMAP_HPP
