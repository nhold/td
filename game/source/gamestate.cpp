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

	sf::Font* buttonFont = new sf::Font(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf"));
	sf::Sprite* towerSprite1 = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower1.png"));
	sf::Sprite* towerSprite2 = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower2.png"));
	sf::Sprite* towerSprite3 = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower3.png"));

	buttonSpawner.AddType(Button(towerSprite1, buttonFont, "", [] {
		
	}));

	buttonSpawner.AddType(Button(towerSprite2, buttonFont, "", [] {

	}));

	buttonSpawner.AddType(Button(towerSprite3, buttonFont, "", [] {

	}));

	auto tower1Button = buttonSpawner.Spawn(0);
	tower1Button->Listen(std::bind(&GameState::UpdateSelectedTower, this, 0));
	tower1Button->node.SetPosition(16, 624);

	auto tower2Button = buttonSpawner.Spawn(1);
	tower2Button->Listen(std::bind(&GameState::UpdateSelectedTower, this, 1));
	tower2Button->node.SetPosition(48, 624);

	auto tower3Button = buttonSpawner.Spawn(2);
	tower3Button->Listen(std::bind(&GameState::UpdateSelectedTower, this, 2));
	tower3Button->node.SetPosition(80, 624);

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

	enemySystem.DespawnAll();

	towerSpawner.DespawnAll();
	projectileSpawner.DespawnAll();
	buttonSpawner.DespawnAll();
	DestroyTypes();
	
	currentLevel.Clear();
}

void GameState::Update()
{
	// Empty
}

void GameState::MultithreadedUpdate()
{
	enemySystem.EnemyKilled = [this](Enemy* b)
	{
		currentGold += b->worth;
	};
	
	while (running)
	{
		auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
		auto worldGridMousePosition = Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(renderWindow)));

		if (mousePosition.x >= 0 && mousePosition.y 
			>= 0 && mousePosition.x <= 640 && mousePosition.y <= 640)
		{
			bool onGui = false;
			for each (auto but in buttonSpawner.instances)
			{
				onGui = but->IsPositionOver(mousePosition);
				if (onGui)
					break;
			}

			auto grid = Game::WorldToArray(mousePosition);

			if (currentLevel.tileMap.tiles[grid.x][grid.y] == 1 ||
				currentLevel.tileMap.tiles[grid.x][grid.y] == 2 ||
				currentLevel.tileMap.tiles[grid.x][grid.y] == 3 ||
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
					if (towerSpawner.types[currentSelectedTower].cost <= currentGold && !onGui)
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

			cursor.setPosition(worldGridMousePosition);
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
		UpdateWave(threadDeltaTime);
		enemySystem.Update(threadDeltaTime);

		// TODO: Pass in the enemyspawner?
		enemySystem.mutex.lock();
		currentLevel.base->Update(enemySystem.instances);
		enemySystem.mutex.unlock();

		UpdateTowers(threadDeltaTime);

		projectileSpawner.mutex.lock();
		for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
		{
			auto projectile = (*it);
			projectile->Update(enemySystem, threadDeltaTime);

			if (!projectile->node.isAlive)
			{
				deadProjectileVector.push_back(projectile);
			}
		}
		projectileSpawner.mutex.unlock();

		if (enemySystem.instances.size() == 0 && currentWave >= currentLevel.waves.size() || currentLevel.base->health <= 0)
		{
			running = false;
		}

		threadDeltaTime = clock.restart().asSeconds();
	}

	stateMachine.SetState(menuState);
}

void GameState::UpdateSelectedTower(int selectedTowerIndex)
{
	currentSelectedTower = std::clamp(selectedTowerIndex, 0, 2);
	towerRadius.setRadius(towerSpawner.types[currentSelectedTower].radius);
	towerRadius.setOrigin(towerSpawner.types[currentSelectedTower].radius, towerSpawner.types[currentSelectedTower].radius);
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

	enemySystem.Render(renderWindow);

	projectileSpawner.mutex.lock();
	for (auto it = projectileSpawner.instances.begin(); it != projectileSpawner.instances.end(); ++it)
	{
		renderWindow.draw(*(*it)->node.GetSprite());
	}
	projectileSpawner.mutex.unlock();


	renderWindow.draw(cursor);
	renderWindow.draw(towerRadius);
	renderWindow.draw(goldText);

	for each (auto but in buttonSpawner.instances)
	{
		renderWindow.draw(*but->node.GetSprite());
	}
}

void GameState::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::MouseButtonReleased)
	{
		if (currentEvent.mouseButton.button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePosition(currentEvent.mouseButton.x, currentEvent.mouseButton.y);

			//static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow));

			Button* hit = nullptr;
			buttonSpawner.IsPositionOver(mousePosition, &hit);

			if (hit != nullptr)
			{
				hit->Invoke();
			}
		}
	}

	if (currentEvent.type == sf::Event::KeyPressed)
	{
		if (currentEvent.key.code == sf::Keyboard::A)
		{
			enemySystem.mutex.lock();
			int dice_roll = distribution(generator);
			enemySystem.Spawn(dice_roll);
			enemySystem.mutex.unlock();
		}

		if (currentEvent.key.code == sf::Keyboard::Space)
		{
			enemySystem.mutex.lock();
			enemySystem.Spawn(2);
			enemySystem.mutex.unlock();
		}

		if (currentEvent.key.code == sf::Keyboard::D)
		{
			enemySystem.mutex.lock();
			enemySystem.DespawnBack();
			enemySystem.mutex.unlock();
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

	// TODO: This should be defined by some tilemap data.
	currentLevel.tileMap.tileTypes[0] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/grass.png").resource);
	currentLevel.tileMap.tileTypes[1] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/dirt.png").resource);
	currentLevel.tileMap.tileTypes[2] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/water.png").resource);
	currentLevel.tileMap.tileTypes[3] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/cliff.png").resource);
}

void GameState::UpdateTowers(float deltaTime)
{
	towerSpawner.mutex.lock();
	for (auto it = towerSpawner.instances.begin(); it != towerSpawner.instances.end(); ++it)
	{
		(*it)->Update(enemySystem, deltaTime);
	}
	towerSpawner.mutex.unlock();
}


void GameState::PostUpdate()
{
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
}

void GameState::UpdateWave(float deltaTime)
{
	if (currentWave < currentLevel.waves.size())
	{
		time -= deltaTime;
		if (time <= 0)
		{
			if (currentData < currentLevel.waves[currentWave].enemySpawnData.size())
			{
				enemySystem.mutex.lock();
				time = currentLevel.waves[currentWave].enemySpawnData[currentData].spawnTime;
				enemySystem.Spawn(currentLevel.waves[currentWave].enemySpawnData[currentData].type);
				currentData++;
				enemySystem.mutex.unlock();
			}
			else
			{
				// TODO: Add pause time?
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

	enemySystem.AddType(enemy1);
	enemySystem.AddType(enemy2);
	enemySystem.AddType(enemy3);

	Tower tower1(0, 1, 5.f, 100.f, 0.5f, 25, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower1.png").resource), "Tower One", projectileSpawner);
	tower1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	towerSpawner.AddType(tower1);

	Tower tower2(1, 1, 6.f, 150.f, 1.5f, 50, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower2.png").resource), "Tower Bomb", projectileSpawner);
	tower1.node.SetFont(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf").resource);
	towerSpawner.AddType(tower2);

	Tower tower3(2, 2, 9.f, 125.f, 2.0f, 75, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/tower3.png").resource), "Tower Arcane", projectileSpawner);
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
	enemySystem.types.clear();
	towerSpawner.types.clear();
	buttonSpawner.types.clear();
}
