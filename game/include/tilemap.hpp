#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>

namespace sf 
{
	class Sprite;
	class RenderWindow;
}

class TileMap
{
public:
	TileMap();
	~TileMap();

	void Render(sf::RenderWindow* window);
	void SaveToFile(const std::string & fileName);
	void LoadFromFile(const std::string& fileName);

	static const int tileMapSize = 50;
	int tiles[TileMap::tileMapSize][TileMap::tileMapSize];
	sf::Sprite* tileTypes[10];

};

#endif
