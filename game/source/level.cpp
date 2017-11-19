#include <level.hpp>

#include <path.hpp>
#include <assethelper.hpp>
#include <game.hpp>
#include <base.hpp>
#include <vector>
#include <enemy.hpp>
#include <tower.hpp>

Level::Level(Spawner<Enemy>& enemySpawner, Spawner<Tower>& towerSpawner, Spawner<Projectile>& projectileSpawner) : enemySpawner(enemySpawner), towerSpawner(towerSpawner), projectileSpawner(projectileSpawner)
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

void Level::Update()
{
	PostUpdate();

	UpdateWave();
	UpdateEnemies();
	base->Update(enemySpawner.instances);
	UpdateTowers();

	for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
	{
		auto projectile = (*it);
		projectile->Update();

		if (!projectile->node.isAlive)
		{
			deadProjectileVector.push_back(projectile);
		}
	}
}

void Level::UpdateTowers()
{
	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		(*it)->Update(enemySpawner.instances);
	}
}

void Level::UpdateEnemies()
{
	for (auto it = enemySpawner.instances.begin(); it != enemySpawner.instances.end(); ++it)
	{
		auto enemy = (*it);
		enemy->Update();

		if (!enemy->node.isAlive)
		{
			deadEnemyVector.push_back(enemy);
		}
	}
}

void Level::PostUpdate()
{
	for (auto it = deadEnemyVector.begin(); it != deadEnemyVector.end(); ++it)
	{
		auto killIt = std::find(enemySpawner.instances.begin(), enemySpawner.instances.end(), (*it));

		if (killIt != enemySpawner.instances.end())
		{
			currentGold += (*killIt)->worth;
			enemySpawner.instances.erase(killIt);
			delete (*it);
		}
	}


	for (auto it = deadProjectileVector.begin(); it != deadProjectileVector.end(); ++it)
	{
		auto killIt = std::find(projectileSpawner.instances.begin(), projectileSpawner.instances.end(), (*it));

		if (killIt != projectileSpawner.instances.end())
		{
			projectileSpawner.instances.erase(killIt);
			delete (*it);
		}
	}

	deadProjectileVector.clear();
	deadEnemyVector.clear();
}

void Level::UpdateWave()
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
}

void Level::Render(sf::RenderWindow & window)
{
	tileMap.Render(&window);
	window.draw(*base->node.GetSprite());
}
