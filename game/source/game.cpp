#include <game.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <vectorutility.hpp>

#include <iostream>

float Game::deltaTime = 0.f;

// TODO Enemy spawner.
/*spawnTimer -= deltaTime;
if (spawnTimer <= 0)
{
spawnTimer = 1.f * enemies.size();
}*/

Game::Game() : distribution(0,1)
{
	window.create(sf::VideoMode(640, 640), "TD");

	tileMap.tileTypes[0] = CreateTempSprite(sf::Color::Green);
	tileMap.tileTypes[1] = CreateTempSprite(sf::Color(125, 68, 29));
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);
	enemySprite = CreateTempSprite(sf::Color::Red, 16, 16);
	tileMap.LoadFromFile("assets/level1.txt");
	tileMap.SaveToFile("assets/tilemapsave.txt");
	path = new Path(tileMap);
	debugFont.loadFromFile("assets/Consolas.ttf");
	
	debugText.setFont(debugFont);
	debugText.setCharacterSize(14);
	debugText.setColor(sf::Color::Magenta);
	CreateTypes();

	cursor.setTexture(GetTexture("assets/cursor.png"));
	cursor.setPosition(0, 0);
	debugEntities = true;
}

Game::~Game()
{
	while (enemies.size() > 0)
	{
		auto enem = enemies.back();
		delete enem;
		enemies.pop_back();
	}

	while (towers.size() > 0)
	{
		auto tower = towers.back();
		delete tower;
		towers.pop_back();
	}
}

void Game::Update()
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
			enemies.erase(killIt);
			delete (*it);
		}
	}

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		(*it)->Update(enemies);
	}
	
	debugText.setString("Delta time: " + std::to_string(deltaTime) + 
		"\n" + "FPS: " + std::to_string(1.f/deltaTime) +
		"\n" + "Mouse Pos: " + ToString(sf::Mouse::getPosition()) +
		"\n" + "Mouse Pressed: " + std::to_string((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))));
	
	auto grid = WorldToArray(sf::Vector2f(sf::Mouse::getPosition(window)));

	if (this->tileMap.tiles[grid.x][grid.y] == 1 || this->buildingMap.isBlocked[grid.x][grid.y])
	{
		cursor.setColor(sf::Color::Red);
	}
	else 
	{
		cursor.setColor(sf::Color::Black);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			this->buildingMap.isBlocked[grid.x][grid.y] = true;
			Tower* tower = new Tower(towerTypes[0]);
			tower->isBuilding = true;
			tower->SetPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
			towers.push_back(tower);
		}
	}

	cursor.setPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
}

void Game::Run()
{
	while (window.isOpen())
	{
		ProcessEvents();
		Update();
		Render();

		deltaTime = clock.restart().asSeconds();
	}
}

sf::Vector2f Game::GridToWorld(sf::Vector2i gridPoint)
{
	sf::Vector2f worldPoint;
	worldPoint.x = gridPoint.x * 32 + 8;
	worldPoint.y = gridPoint.y * 32 + 8;
	return worldPoint;
}

sf::Vector2f Game::WorldToGrid(sf::Vector2f worldPosition)
{
	sf::Vector2f gridPoint;
	gridPoint.x = static_cast<int>((worldPosition.x / 32)) * 32;
	gridPoint.y = static_cast<int>((worldPosition.y / 32)) * 32;
	return gridPoint;
}

sf::Vector2i Game::WorldToArray(sf::Vector2f worldPosition)
{
	sf::Vector2i gridPoint;
	gridPoint.x = static_cast<int>((worldPosition.x / 32));
	gridPoint.y = static_cast<int>((worldPosition.y / 32));
	return gridPoint;
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::A)
			{
				int dice_roll = distribution(generator);
				auto eT = enemyTypes[dice_roll];
				eT.SetName(eT.GetName() + " --ID:-- " + std::to_string(enemies.size()));
				enemies.push_back(new Enemy(eT));
			}

			if (event.key.code == sf::Keyboard::D)
			{
				if (enemies.size() > 0)
				{
					auto enemy = enemies.back();
					enemies.pop_back();
					delete enemy;
				}
			}

			if (event.key.code == sf::Keyboard::C)
			{
				debugEntities = false;
			}

			if (event.key.code == sf::Keyboard::Z)
			{
				debugEntities = true;
			}
		}
	}
}

void Game::CreateTypes()
{
	Enemy enemy1(50, 25, 5, new sf::Sprite(GetTexture("assets/enemy1.png")), path, "Simpleton");
	enemy1.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy1.SetFont(debugFont);

	Enemy enemy2(50, 25, 5, new sf::Sprite(GetTexture("assets/enemy2.png")), path, "Blarg");
	enemy2.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy2.SetFont(debugFont);

	enemyTypes.push_back(enemy1);
	enemyTypes.push_back(enemy2);

	Tower tower1(2, 1, 5.f, 50.f, 1.f, new sf::Sprite(GetTexture("assets/tower1.png")), "Tower One");
	tower1.SetFont(debugFont);
	towerTypes.push_back(tower1);
}

void Game::Render()
{
	window.clear(sf::Color::Magenta);
	tileMap.Render(&window);


	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		window.draw(*(*it)->GetSprite());
	}

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		window.draw(*(*it)->GetSprite());
	}

	if (debugEntities)
	{
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			window.draw((*it)->GetText());
		}

		for (auto it = towers.begin(); it != towers.end(); ++it)
		{
			window.draw((*it)->GetText());
		}
	}

	window.draw(cursor);

	window.draw(debugText);
	window.display();
}

// TODO: Extract to an asset helper class.
sf::Sprite* Game::CreateTempSprite(const sf::Color &color, int length, int height) const
{
	sf::Texture texture;
	texture.create(length, height);
	sf::Image image;
	image.create(length, height, color);
	texture.update(image, 0, 0);

	auto sprite = new sf::Sprite(texture);
	sprite->setColor(color);
	
	return sprite;
}

sf::Texture& Game::GetTexture(std::string fileName)
{
	auto it = textureCache.find(fileName);

	if (it != textureCache.end())
		return (*it).second;

	sf::Texture tex;
	tex.loadFromFile(fileName);
	textureCache[fileName] = tex;

	return textureCache[fileName];
}
