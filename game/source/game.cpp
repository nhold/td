#include <game.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <vectorutility.hpp>

#include <iostream>

float Game::deltaTime = 0.f;

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
}

Game::~Game()
{
}

void Game::Update()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		(*it).Update();
	}
	
	debugText.setString("Delta time: " + std::to_string(deltaTime) + 
		"\n" + "FPS: " + std::to_string(1.f/deltaTime) +
		"\n" + "Mouse Pos: " + ToString(sf::Mouse::getPosition()));
	
	cursor.setPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
	/*spawnTimer -= deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = 1.f * enemies.size();

		
		
	}*/
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
				enemies.push_back(eT);
			}

			if (event.key.code == sf::Keyboard::D)
			{
				enemies.pop_back();
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
}

void Game::Render()
{
	window.clear(sf::Color::Magenta);
	tileMap.Render(&window);


	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		window.draw((*it).GetText());
	}

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		window.draw(*(*it).GetSprite());
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
