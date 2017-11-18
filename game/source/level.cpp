#include <level.hpp>

#include <path.hpp>
#include <assethelper.hpp>
#include <game.hpp>
#include <base.hpp>
#include <vector>
#include <enemy.hpp>
#include <tower.hpp>

Level::Level()
{
	path = nullptr;
	startingGold = 1337;
}

Level::~Level()
{
	if (path != nullptr)
		delete path;
}

void Level::Load(std::string tileMapFileName, AssetDatabase& assetDatabase)
{
	tileMap.LoadFromFile(tileMapFileName);
	currentGold = startingGold;
	path = new Path(tileMap);
	base = new Base(100, new sf::Sprite(assetDatabase.GetTexture("assets/base.png")), "Base");
	base->node.SetPosition(Game::GridToWorld(path->nodePoints.back()));

	tileMap.tileTypes[0] = new sf::Sprite(assetDatabase.GetTexture("assets/grass.png"));
	tileMap.tileTypes[1] = new sf::Sprite(assetDatabase.GetTexture("assets/dirt.png"));
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);
}

void Level::Update(std::vector<Enemy*>& enemies, std::vector<Tower*>& towers)
{
	std::vector<Enemy*> deadVector;

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		(*it)->Update();
		if ((*it)->currentHealth <= 0)
		{
			deadVector.push_back((*it));
		}
	}

	for (auto it = deadVector.begin(); it != deadVector.end(); ++it)
	{
		auto killIt = std::find(enemies.begin(), enemies.end(), (*it));

		if (killIt != enemies.end())
		{
			currentGold += (*killIt)->worth;
			enemies.erase(killIt);
			delete (*it);
		}
	}

	base->Update(enemies);

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		(*it)->Update(enemies);
	}
}

void Level::Render(sf::RenderWindow & window)
{
	tileMap.Render(&window);
	window.draw(*base->node.GetSprite());
}
