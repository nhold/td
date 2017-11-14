#include <game.hpp>

#include <iostream>

#include <path.hpp>
#include <enemy.hpp>
#include <vectorutility.hpp>
#include <base.hpp>



float Game::deltaTime = 0.f;

// TODO Enemy spawner.
/*spawnTimer -= deltaTime;
if (spawnTimer <= 0)
{
spawnTimer = 1.f * enemies.size();
}*/

Game::Game() : distribution(0, 1)
{
	window.create(sf::VideoMode(640, 640), "TD");

	gold = 100;

	tileMap.tileTypes[0] = CreateTempSprite(sf::Color::Green);
	tileMap.tileTypes[1] = CreateTempSprite(sf::Color(125, 68, 29));
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);
	enemySprite = CreateTempSprite(sf::Color::Red, 16, 16);

	tileMap.LoadFromFile("assets/level1.txt");
	tileMap.SaveToFile("assets/tilemapsave.txt");
	debugFont.loadFromFile("assets/Consolas.ttf");

	path = new Path(tileMap);

	debugText.setFont(debugFont);
	debugText.setCharacterSize(14);
	debugText.setColor(sf::Color::Magenta);

	goldText.setFont(debugFont);
	goldText.setCharacterSize(14);
	goldText.setColor(sf::Color::Blue);

	CreateTypes();

	cursor.setTexture(GetTexture("assets/cursor.png"));
	cursor.setPosition(0, 0);
	cursor.setOrigin(16, 16);
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
	// Best to extract this to enemy system?
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
			gold += (*killIt)->worth;
			goldText.setString("Gold: " + std::to_string(gold));
			enemies.erase(killIt);
			delete (*it);
		}
	}

	base->Update(enemies);

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		(*it)->Update(enemies);
	}

	debugText.setString("\n\n\n\n\nDelta time: " + std::to_string(deltaTime) +
		"\n" + "FPS: " + std::to_string(1.f / deltaTime) +
		"\n" + "Mouse Pos: " + ToString(sf::Mouse::getPosition()) +
		"\n" + "Mouse Pressed: " + std::to_string((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))));

	auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	
	goldText.setString("Gold: " + std::to_string(gold) + "\n" + "Health: " + std::to_string(base->health));

	if (mousePosition.x >= 0 || mousePosition.y >= 0)
	{
		auto grid = WorldToArray(mousePosition);

		if (this->tileMap.tiles[grid.x][grid.y] == 1 || this->buildingMap.isBlocked[grid.x][grid.y])
		{
			cursor.setColor(sf::Color::Red);
		}
		else
		{
			cursor.setColor(sf::Color::Black);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (towerTypes[0].cost <= gold)
				{
					this->buildingMap.isBlocked[grid.x][grid.y] = true;
					Tower* tower = new Tower(towerTypes[0]);
					tower->isBuilding = true;
					tower->SetPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
					towers.push_back(tower);
					gold -= towerTypes[0].cost;
					goldText.setString("Gold: " + std::to_string(gold));
					std::cout << "What is happening" << std::endl;
				}
				else
				{
					cursor.setColor(sf::Color::Blue);
				}
			}
		}

		cursor.setPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
	}
}

void Game::Run()
{
	Initialise();

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
	worldPoint.x = gridPoint.x * 32 + 16;
	worldPoint.y = gridPoint.y * 32 + 16;
	return worldPoint;
}

sf::Vector2f Game::WorldToGrid(sf::Vector2f worldPosition)
{
	return GridToWorld(WorldToArray(worldPosition));
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

			if (event.key.code == sf::Keyboard::Space)
			{
				auto eT = new Enemy(enemyTypes[2]);
				eT->SetName(eT->GetName() + " --ID:-- " + std::to_string(enemies.size()));
				enemies.push_back(eT);
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
	Enemy enemy1(10, 50, 5, 5, new sf::Sprite(GetTexture("assets/enemy1.png")), path, "Simpleton");
	enemy1.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy1.SetFont(debugFont);

	Enemy enemy2(10, 100, 5, 10, new sf::Sprite(GetTexture("assets/enemy2.png")), path, "Blarg");
	enemy2.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy2.SetFont(debugFont);

	Enemy enemy3(1000, 25, 50, 120, new sf::Sprite(GetTexture("assets/demon.png")), path, "Demon");
	enemy3.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy3.SetFont(debugFont);

	enemyTypes.push_back(enemy1);
	enemyTypes.push_back(enemy2);
	enemyTypes.push_back(enemy3);

	Tower tower1(2, 1, 5.f, 100.f, 1.f, 25, new sf::Sprite(GetTexture("assets/tower1.png")), "Tower One");
	tower1.SetFont(debugFont);
	towerTypes.push_back(tower1);

	base = new Base(100, new sf::Sprite(GetTexture("assets/base.png")), "Base");
	base->node.SetPosition(GridToWorld(path->nodePoints.back()));
}

void Game::Initialise()
{
	goldText.setString("Gold: " + std::to_string(gold));
}

void Game::Render()
{
	window.clear(sf::Color::Magenta);
	tileMap.Render(&window);

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		window.draw(*(*it)->GetSprite());
	}

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		window.draw(*(*it)->GetSprite());
	}

	window.draw(*base->node.GetSprite());

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
	window.draw(goldText);
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
