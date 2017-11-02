#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "tilemap.hpp"

class Game
{
public:
	Game();
	~Game();

	void Run();
private:
	sf::RenderWindow window;

	void ProcessEvents();
	void Render();
	TileMap tileMap;

	sf::Sprite* CreateTempSprite(const sf::Color &color) const;
};

#endif
