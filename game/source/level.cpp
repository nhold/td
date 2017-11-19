#include <level.hpp>

#include <path.hpp>
#include <assethelper.hpp>
#include <game.hpp>
#include <base.hpp>
#include <vector>
#include <enemy.hpp>
#include <tower.hpp>

Level::Level(EnemySpawner& enemySpawner) : enemySpawner(enemySpawner)
{
	path = nullptr;
	startingGold = 100;
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
	
	currentWave = 0;
	currentData = 0;
	Wave wave;
	wave.LoadFromFile("assets/level1_wavedata.txt");
	waves.push_back(wave);
}

void Level::Update(std::vector<Enemy*>& enemies, std::vector<Tower*>& towers)
{
	if (currentWave < waves.size())
	{
		time -= Game::deltaTime;
		if (time <= 0)
		{
			if (currentData < waves[currentWave].enemySpawnData.size())
			{
				time = waves[currentWave].enemySpawnData[currentData].spawnTime;
				enemySpawner.Spawn(waves[currentWave].enemySpawnData[currentData].type);
				currentData++;
			}
			else
			{
				currentWave++;
			}
		}
	}

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
