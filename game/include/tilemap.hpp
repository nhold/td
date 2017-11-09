#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <SFML/System/Vector2.hpp>

namespace sf 
{
	class Sprite;
	class RenderWindow;
}

class Path;

//TODO: Should a tilemap be a loaded from file with characters?
class TileMap
{
public:
	TileMap();
	~TileMap();

	void Render(sf::RenderWindow* window);
	void SaveToFile(const std::string & fileName);
	void LoadFromFile(const std::string& fileName);
	bool WithinBounds(sf::Vector2i point);
	static const int tileMapSize = 20;
	int tiles[TileMap::tileMapSize][TileMap::tileMapSize];
	sf::Sprite* tileTypes[10];

private:
	Path* path;
};

#endif
