#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <tilemap.hpp>
#include <map>
#include <random>

class Path;
class Enemy;

class Game
{
public:
	Game();
	~Game();

	void Run();

	static float Game::deltaTime;
	static sf::Vector2f GridToWorld(sf::Vector2i gridPoint);
	static sf::Vector2f WorldToGrid(sf::Vector2f worldPoint);

private:
	sf::RenderWindow window;
	sf::Sprite* enemySprite;
	int currentNode = 0;
	float frameWait = 0.2f;
	void ProcessEvents();
	void CreateTypes();
	void Update();
	void Render();
	sf::Clock clock;
	TileMap tileMap;
	Path* path;
	sf::Sprite* CreateTempSprite(const sf::Color &color, int length = 32, int height = 32) const;
	sf::Text debugText;
	sf::Font debugFont;

	std::vector<Enemy> enemies;
	std::vector<Enemy> enemyTypes;
	float spawnTimer = 2.5f;
	sf::Texture& GetTexture(std::string fileName);
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
	std::map<std::string, sf::Texture> textureCache;
	sf::Sprite cursor;
};

#endif
