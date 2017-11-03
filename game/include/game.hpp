#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <tilemap.hpp>

class Path;

class Game
{
public:
	Game();
	~Game();

	void Run();
private:
	sf::RenderWindow window;
	sf::Sprite* enemySprite;
	int currentNode = 0;
	int frameWait = 600;
	void ProcessEvents();
	void Render();
	TileMap tileMap;
	Path* path;
	sf::Sprite* CreateTempSprite(const sf::Color &color, int length = 32, int height = 32) const;
};

#endif
