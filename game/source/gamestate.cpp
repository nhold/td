#include <gamestate.hpp>

#include <level.hpp>
#include <base.hpp>
#include <tower.hpp>
#include <game.hpp>
#include <path.hpp>
#include <assethelper.hpp>
#include <menustate.hpp>

#include <chrono>

GameState::GameState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow) : assetDatabase(assetDatabase), 
	renderWindow(renderWindow), 
	distribution(0, 1), 
	currentLevel(assetDatabase), 
	stateMachine(stateMachine)
{
	currentGold = 1;
	currentSelectedTower = 0;
}

GameState::~GameState()
{
	Shutdown();
}

void GameState::Initialise()
{
	if (!currentLevel.isValid())
	{
		stateMachine.SetState(menuState);
		return;
	}

	running = true;
	currentSelectedTower = 0;
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
	updateThread = new std::thread(std::bind(&GameState::MultithreadedUpdate, this));
	threadDeltaTime = 0.0f;
}

void GameState::Shutdown()
{
	running = false;
	//updateThread->join(); - Fix this.

	enemySpawner.DespawnAll();
	towerSpawner.DespawnAll();
	projectileSpawner.DespawnAll();
	DestroyTypes();
	
	currentLevel.Clear();
}

void GameState::MultithreadedUpdate()
{
	while (running)
	{
		auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
		auto worldGridMousePosition = Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow)));

		if (mousePosition.x >= 0 && mousePosition.y >= 0 && mousePosition.x <= 640 && mousePosition.y <= 640)
		{
			auto grid = Game::WorldToArray(mousePosition);

			if (currentLevel.tileMap.tiles[grid.x][grid.y] == 1 ||
				currentLevel.tileMap.tiles[grid.x][grid.y] == 2 ||
				currentLevel.buildingMap.isBlocked[grid.x][grid.y])
			{
				cursor.setColor(sf::Color::Red);
			}
			else
			{
				cursor.setColor(sf::Color::Black);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					towerSpawner.mutex.lock();
					if (towerSpawner.types[currentSelectedTower].cost <= currentGold)
					{
						currentLevel.buildingMap.isBlocked[grid.x][grid.y] = true;
						Tower* tower = towerSpawner.Spawn(currentSelectedTower);
						tower->isBuilding = true;
						tower->node.SetPosition(worldGridMousePosition);

						currentGold -= towerSpawner.types[currentSelectedTower].cost;
					}
					else
					{
						cursor.setColor(sf::Color::Blue);
					}
					towerSpawner.mutex.unlock();
				}
			}

			cursor.setPosition(Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow))));
			towerRadius.setPosition(worldGridMousePosition);
		}

		int displayCurrentWave = std::min(currentWave + 1, (int)currentLevel.waves.size());
		int displayCurrentData = std::min(currentData + 1, (int)currentLevel.waves[displayCurrentWave - 1].enemySpawnData.size());
		goldText.setString("Gold: " + std::to_string(currentGold) +
			"\n" + "Health: " + std::to_string(currentLevel.base->health) +
			"\n" + "Wave: " + std::to_string(displayCurrentWave) + "/" + std::to_string(currentLevel.waves.size()) +
			"\n" + "Enemies: " + std::to_string(displayCurrentData) + "/" + std::to_string(currentLevel.waves[displayCurrentWave - 1].enemySpawnData.size()) +
			"\n" + "Next Enemy: " + std::to_string(time));

		PostUpdate();

		UpdateWave();
		UpdateEnemies(threadDeltaTime);

		enemySpawner.mutex.lock();
		currentLevel.base->Update(enemySpawner.instances);
		enemySpawner.mutex.unlock();

		UpdateTowers(threadDeltaTime);

		projectileSpawner.mutex.lock();
		for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
		{
			auto projectile = (*it);
			projectile->Update(enemySpawner, threadDeltaTime);

			if (!projectile->node.isAlive)
			{
				deadProjectileVector.push_back(projectile);
			}
		}
		projectileSpawner.mutex.unlock();

		if (enemySpawner.instances.size() == 0 && currentWave >= currentLevel.waves.size() || currentLevel.base->health <= 0)
		{
			running = false;
		}

		threadDeltaTime = clock.restart().asSeconds();
	}

	stateMachine.SetState(menuState);
}

void GameState::Update()
{
	/*auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
	auto worldGridMousePosition = Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow)));

	if (mousePosition.x >= 0 && mousePosition.y >= 0 && mousePosition.x <= 640 && mousePosition.y <= 640)
	{
		auto grid = Game::WorldToArray(mousePosition);

		if (currentLevel.tileMap.tiles[grid.x][grid.y] == 1 ||
			currentLevel.tileMap.tiles[grid.x][grid.y] == 2 ||
			currentLevel.buildingMap.isBlocked[grid.x][grid.y])
		{
			cursor.setColor(sf::Color::Red);
		}
		else
		{
			cursor.setColor(sf::Color::Black);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				towerSpawner.mutex.lock();
				if (towerSpawner.types[currentSelectedTower].cost <= currentGold)
				{
					currentLevel.buildingMap.isBlocked[grid.x][grid.y] = true;
					Tower* tower = towerSpawner.Spawn(currentSelectedTower);
					tower->isBuilding = true;
					tower->node.SetPosition(worldGridMousePosition);

					currentGold -= towerSpawner.types[currentSelectedTower].cost;
				}
				else
				{
					cursor.setColor(sf::Color::Blue);
				}
				towerSpawner.mutex.unlock();
			}
		}

		cursor.setPosition(Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow))));
		towerRadius.setPosition(worldGridMousePosition);
	}

	int displayCurrentWave = std::min(currentWave + 1, (int)currentLevel.waves.size());
	int displayCurrentData = std::min(currentData + 1, (int)currentLevel.waves[displayCurrentWave - 1].enemySpawnData.size());
	goldText.setString("Gold: " + std::to_string(currentGold) +
		"\n" + "Health: " + std::to_string(currentLevel.base->health) +
		"\n" + "Wave: " + std::to_string(displayCurrentWave) + "/" + std::to_string(currentLevel.waves.size()) +
		"\n" + "Enemies: " + std::to_string(displayCurrentData) + "/" + std::to_string(currentLevel.waves[displayCurrentWave - 1].enemySpawnData.size()) +
		"\n" + "Next Enemy: " + std::to_string(time));

	PostUpdate();

	UpdateWave();
	UpdateEnemies();
	currentLevel.base->Update(enemySpawner.instances);
	UpdateTowers();

	projectileSpawner.mutex.lock();
	for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
	{
		auto projectile = (*it);
		projectile->Update(enemySpawner);

		if (!projectile->node.isAlive)
		{
			deadProjectileVector.push_back(projectile);
		}
	}
	projectileSpawner.mutex.unlock();

	if (enemySpawner.instances.size() == 0 && currentWave >= currentLevel.waves.size())
	{
		stateMachine.SetState(menuState);
	}*/
	
}

void GameState::Render()
{
	currentLevel.Render(renderWindow);

	towerSpawner.mutex.lock();
	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
	}
	towerSpawner.mutex.unlock();

	enemySpawner.mutex.lock();
	for (auto it = enemySpawner.instances.begin(); it != enemySpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
		(*it)->RenderHealthbars(renderWindow);
	}
	enemySpawner.mutex.unlock();

	projectileSpawner.mutex.lock();
	for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
	}
	projectileSpawner.mutex.unlock();


	renderWindow.draw(cursor);
	renderWindow.draw(towerRadius);
	renderWindow.draw(goldText);
}

void GameState::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::KeyPressed)
	{
		if (currentEvent.key.code == sf::Keyboard::A)
		{
			enemySpawner.mutex.lock();
			int dice_roll = distribution(generator);
			enemySpawner.Spawn(dice_roll);
			enemySpawner.mutex.unlock();
		}

		if (currentEvent.key.code == sf::Keyboard::Space)
		{
			enemySpawner.mutex.lock();
			enemySpawner.Spawn(2);
			enemySpawner.mutex.unlock();
		}

		if (currentEvent.key.code == sf::Keyboard::D)
		{
			enemySpawner.mutex.lock();
			enemySpawner.DespawnBack();
			enemySpawner.mutex.unlock();
		}

		if (currentEvent.key.code == sf::Keyboard::Escape)
		{
			running = false;
			updateThread->join();
			stateMachine.SetState(menuState);
		}

		if (currentEvent.key.code == sf::Keyboard::Num1)
		{
			currentSelectedTower = 0;
			towerRadius.setRadius(towerSpawner.types[0].radius);
			towerRadius.setOrigin(towerSpawner.types[0].radius, towerSpawner.types[0].radius);
		}

		if (currentEvent.key.code == sf::Keyboard::Num2)
		{
			currentSelectedTower = 1;
			towerRadius.setRadius(towerSpawner.types[1].radius);
			towerRadius.setOrigin(towerSpawner.types[1].radius, towerSpawner.types[1].radius);
		}

		if (currentEvent.key.code == sf::Keyboard::Num3)
		{
			currentSelectedTower = 2;
			towerRadius.setRadius(towerSpawner.types[2].radius);
			towerRadius.setOrigin(towerSpawner.types[2].radius, towerSpawner.types[2].radius);
		}

		if (currentEvent.key.code == sf::Keyboard::Escape)
		{
			stateMachine.SetState(menuState);
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

void GameState::UpdateTowers(float deltaTime)
{
	towerSpawner.mutex.lock();
	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		(*it)->Update(enemySpawner, deltaTime);
	}
	towerSpawner.mutex.unlock();
}

void GameState::UpdateEnemies(float deltaTime)
{
	enemySpawner.mutex.lock();
	for (auto it = enemySpawner.instances.begin(); it != enemySpawner.instances.end(); ++it)
	{
		auto enemy = (*it);
		enemy->Update(deltaTime);

		if (!enemy->node.isAlive)
		{
			deadEnemyVector.push_back(enemy);
		}
	}
	enemySpawner.mutex.unlock();
}

void GameState::PostUpdate()
{

	for (auto it = deadEnemyVector.begin(); it != deadEnemyVector.end(); ++it)
	{
		enemySpawner.mutex.lock();
		auto killIt = std::find(enemySpawner.instances.begin(), enemySpawner.instances.end(), (*it));

		if (killIt != enemySpawner.instances.end())
		{
			currentGold += (*killIt)->worth;
			enemySpawner.instances.erase(killIt);
			delete (*it);
		}
		enemySpawner.mutex.unlock();
	}


	for (auto it = deadProjectileVector.begin(); it != deadProjectileVector.end(); ++it)
	{
		projectileSpawner.mutex.lock();
		auto killIt = std::find(projectileSpawner.instances.begin(), projectileSpawner.instances.end(), (*it));

		if (killIt != projectileSpawner.instances.end())
		{
			projectileSpawner.instances.erase(killIt);
			delete (*it);
		}
		projectileSpawner.mutex.unlock();
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
				enemySpawner.mutex.lock();
				time = currentLevel.waves[currentWave].enemySpawnData[currentData].spawnTime;
				enemySpawner.Spawn(currentLevel.waves[currentWave].enemySpawnData[currentData].type);
				currentData++;
				enemySpawner.mutex.unlock();
			}
			else
			{
				// TODO: Add pause time.
				currentWave++;
				currentData = 0;
			}
		}
	}
}

void GameState::CreateTypes()
{
	// TODO: Load types from file.
	Enemy enemy1(13, 50, 5, 4, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/enemy1.png").resource), currentLevel.path, "Simpleton");
	enemy1.node.SetPosition(Game::GridToWorld(currentLevel.path->nodePoints[0]));
	enemy1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);

	Enemy enemy2(7, 100, 5, 8, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/enemy2.png").resource), currentLevel.path, "Blarg");
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

	Tower tower2(1, 1, 6.f, 150.f, 1.5f, 50, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower2.png").resource), "Tower Bomb", projectileSpawner);
	tower1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	towerSpawner.AddType(tower2);

	Tower tower3(2, 2, 9.f, 125.f, 0.9f, 75, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower3.png").resource), "Tower Arcane", projectileSpawner);
	tower3.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	towerSpawner.AddType(tower3);

	Projectile projectile(250, 5, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/projectile1.png").resource), false, 0, 0, 0.f, "Arrow", projectileSpawner);
	projectile.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	projectileSpawner.AddType(projectile);

	Projectile projectile2(150, 2, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/projectile2.png").resource), true, 115.f, 0, 0.f, "Bomb", projectileSpawner);
	projectile2.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	projectileSpawner.AddType(projectile2);

	Projectile projectile3(200, 2, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/projectile3.png").resource), false, 0.f, 4, 125.f, "Blast", projectileSpawner);
	projectile3.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	projectileSpawner.AddType(projectile3);
}

void GameState::DestroyTypes()
{
	projectileSpawner.types.clear();
	enemySpawner.types.clear();
	towerSpawner.types.clear();
}
