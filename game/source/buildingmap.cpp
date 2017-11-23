#include <buildingmap.hpp>

BuildingMap::BuildingMap()
{
	for (int x = 0; x < TileMap::tileMapSize; x++)
	{
		for (int y = 0; y < TileMap::tileMapSize; y++)
		{
			isBlocked[x][y] = false;
		}
	}
}

void BuildingMap::Clear()
{
	for (int x = 0; x < TileMap::tileMapSize; x++)
	{
		for (int y = 0; y < TileMap::tileMapSize; y++)
		{
			isBlocked[x][y] = false;
		}
	}
}
