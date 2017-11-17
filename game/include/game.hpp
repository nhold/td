#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <tilemap.hpp>
#include <buildingmap.hpp>
#include <tower.hpp>
#include <map>
#include <random>

class Path;
class Enemy;
class Tower;
class Base;

class Game
{
public:
	Game();
	Game(int width, int height, std::string title);

	~Game();

	void Run();
	sf::Texture& GetTexture(std::string fileName);
	sf::Sprite* CreateTempSprite(const sf::Color &color, int length = 32, int height = 32) const;
	static float Game::deltaTime;
	static sf::Vector2f GridToWorld(sf::Vector2i gridPoint);
	static sf::Vector2f WorldToGrid(sf::Vector2f worldPosition);
	static sf::Vector2i WorldToArray(sf::Vector2f worldPosition);

protected:
	sf::Text debugText;
	sf::Font debugFont;
	sf::RenderWindow window;

	virtual void Initialise();
	virtual void Update();
	virtual void Render();
	virtual void ProcessInput(sf::Event currentEvent);


private:
	void ProcessEvents();
	
	sf::Clock clock;

	std::map<std::string, sf::Texture> textureCache;
	
	bool showDebugText;
};

#endif
