#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <tilemap.hpp>

class Path 
{
public:
	Path(TileMap& tileMap);
	std::vector<sf::Vector2i> nodePoints;

	bool HasPoint(sf::Vector2i point)
	{
		for each (auto checkPoint in nodePoints)
		{
			if (checkPoint == point)
				return true;
		}

		return false;
	}
};

#endif // !PATH_HPP
