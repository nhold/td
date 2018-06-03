#include <path.hpp>

Path::Path(TileMap& tileMap)
{
	bool foundFirst = false;
	for (int y = 0; y < TileMap::tileMapSize; y++)
	{
		if (foundFirst == true)
			break;

		for (int x = 0; x < TileMap::tileMapSize; x++)
		{
			if (tileMap.tiles[tileMap.index(x,y)] == 1)
			{
				nodePoints.push_back(sf::Vector2i(x, y));
				foundFirst = true;
				break;
			}
		}
	}

	bool searching = true;
	while (searching)
	{
		auto current = nodePoints.back();

		auto right = current + sf::Vector2i(1, 0);
		auto left = current + sf::Vector2i(-1, 0);
		auto up = current + sf::Vector2i(0, -1);
		auto down = current + sf::Vector2i(0, 1);

		if (tileMap.WithinBounds(right))
		{
			if (tileMap.tiles[tileMap.index(right.x, right.y)] == 1 && !HasPoint(right))
			{
				nodePoints.push_back(right);
				continue;
			}
		}

		if (tileMap.WithinBounds(left))
		{
			if (tileMap.tiles[tileMap.index(left.x,left.y)] == 1 && !HasPoint(left))
			{
				nodePoints.push_back(left);
				continue;
			}
		}

		if (tileMap.WithinBounds(up))
		{
			if (tileMap.tiles[tileMap.index(up.x, up.y)] == 1 && !HasPoint(up))
			{
				nodePoints.push_back(up);
				continue;
			}
		}

		if (tileMap.WithinBounds(down))
		{
			if (tileMap.tiles[tileMap.index(down.x, down.y)] == 1 && !HasPoint(down))
			{
				nodePoints.push_back(down);
				continue;
			}
		}

		searching = false;
	}
}
