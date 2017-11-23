#include <gamestate.hpp>

#include <level.hpp>
#include <base.hpp>
#include <tower.hpp>
#include <game.hpp>
#include <path.hpp>
#include <assethelper.hpp>

GameState::GameState(AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow) : assetDatabase(assetDatabase), renderWindow(renderWindow), distribution(0, 1), currentLevel(assetDatabase)
{
	currentGold = 1;
}

GameState::~GameState()
{
	Shutdown();
}

void GameState::Initialise()
{
	if (!currentLevel.isValid())
	{
		// Put the menu state back in.
	}

	CreateTypes();

	goldText.setFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	goldText.setCharacterSize(14);
	goldText.setFillColor(sf::Color::Blue);

	cursor.setTexture(assetDatabase.textureHandler.GetResource("assets/cursor.png").resource);
	cursor.setPosition(0, 0);
	cursor.setOrigin(16, 16);

	towerRadius.setRadius(towerSpawner.types[0].radius);
	towerRadius.setOrigin(towerSpawner.types[0].radius, towerSpawner.types[0].radius);
	towerRadius.setFillColor(sf::Color::Transparent);
	towerRadius.setOutlineColor(sf::Color::Black);
	towerRadius.setOutlineThickness(2.f);
}

void GameState::Shutdown()
{
	enemySpawner.DespawnAll();
	towerSpawner.DespawnAll();
	projectileSpawner.DespawnAll();
}

void GameState::Update()
{
	auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
	auto worldGridMousePosition = Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow)));

	if (mousePosition.x >= 0 || mousePosition.y >= 0)
	{
		auto grid = Game::WorldToArray(mousePosition);

		if (currentLevel.tileMap.tiles[grid.x][grid.y] == 1 || currentLevel.buildingMap.isBlocked[grid.x][grid.y])
		{
			cursor.setColor(sf::Color::Red);
		}
		else
		{
			cursor.setColor(sf::Color::Black);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (towerSpawner.types[0].cost <= currentGold)
				{
					currentLevel.buildingMap.isBlocked[grid.x][grid.y] = true;
					Tower* tower = towerSpawner.Spawn(0);
					tower->isBuilding = true;
					tower->node.SetPosition(worldGridMousePosition);

					currentGold -= towerSpawner.types[0].cost;
				}
				else
				{
					cursor.setColor(sf::Color::Blue);
				}
			}
		}

		cursor.setPosition(Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow))));
		towerRadius.setPosition(worldGridMousePosition);
	}

	goldText.setString("Gold: " + std::to_string(currentGold) + "\n" + "Health: " + std::to_string(currentLevel.base->health));

	PostUpdate();

	UpdateWave();
	UpdateEnemies();
	currentLevel.base->Update(enemySpawner.instances);
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

void GameState::Render()
{
	currentLevel.Render(renderWindow);

	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
		//window.draw((*it)->GetDebugLines());
	}

	for (auto it = enemySpawner.instances.begin(); it != enemySpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
	}

	for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
	}

	renderWindow.draw(cursor);
	renderWindow.draw(towerRadius);
	renderWindow.draw(goldText);
}

void GameState::ProcessInput(sf::Event currentEvent)
{
	// Debug Keys.
	if (currentEvent.type == sf::Event::KeyPressed)
	{
		if (currentEvent.key.code == sf::Keyboard::A)
		{
			int dice_roll = distribution(generator);
			enemySpawner.Spawn(dice_roll);
		}

		if (currentEvent.key.code == sf::Keyboard::Space)
		{
			enemySpawner.Spawn(2);
		}

		if (currentEvent.key.code == sf::Keyboard::D)
		{
			enemySpawner.DespawnBack();
		}
	}
}

void GameState::SetLevel(std::string levelFileName)
{
	currentLevel.Load(levelFileName);
	currentGold = currentLevel.startingGold;
	currentWave = 0;
	currentData = 0;
	currentLevel.tileMap.tileTypes[0] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/grass.png").resource);
	currentLevel.tileMap.tileTypes[1] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/dirt.png").resource);
	currentLevel.tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);
}

void GameState::UpdateTowers()
{
	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		(*it)->Update(enemySpawner.instances);
	}
}

void GameState::UpdateEnemies()
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

void GameState::PostUpdate()
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

void GameState::UpdateWave()
{
	if (currentWave < currentLevel.waves.size())
	{
		time -= Game::deltaTime;
		if (time <= 0)
		{
			if (currentData < currentLevel.waves[currentWave].enemySpawnData.size())
			{
				time = currentLevel.waves[currentWave].enemySpawnData[currentData].spawnTime;
				enemySpawner.Spawn(currentLevel.waves[currentWave].enemySpawnData[currentData].type);
				currentData++;
			}
			else
			{
				// TODO: Add pause time.
				currentWave++;
			}
		}
	}
}

void GameState::CreateTypes()
{
	Enemy enemy1(10, 50, 5, 5, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/enemy1.png").resource), currentLevel.path, "Simpleton");
	enemy1.node.SetPosition(Game::GridToWorld(currentLevel.path->nodePoints[0]));
	enemy1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);

	Enemy enemy2(5, 100, 5, 10, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/enemy2.png").resource), currentLevel.path, "Blarg");
	enemy2.node.SetPosition(Game::GridToWorld(currentLevel.path->nodePoints[0]));
	enemy2.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);

	Enemy enemy3(1000, 25, 50, 120, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/demon.png").resource), currentLevel.path, "Demon");
	enemy3.node.SetPosition(Game::GridToWorld(currentLevel.path->nodePoints[0]));
	enemy3.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);

	enemySpawner.AddType(enemy1);
	enemySpawner.AddType(enemy2);
	enemySpawner.AddType(enemy3);

	Tower tower1(0, 1, 5.f, 100.f, 1.f, 25, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower1.png").resource), "Tower One", projectileSpawner);
	tower1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	towerSpawner.AddType(tower1);

	Projectile projectile(250, 5, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/projectile1.png").resource), nullptr, "Test");
	projectile.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	projectileSpawner.AddType(projectile);
}
