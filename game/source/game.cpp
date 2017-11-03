#include <game.hpp>
#include <path.hpp>

Game::Game()
{
	window.create(sf::VideoMode(640, 480), "TD");

	tileMap.tileTypes[0] = CreateTempSprite(sf::Color::Green);
	tileMap.tileTypes[1] = CreateTempSprite(sf::Color::Black);
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);
	enemySprite = CreateTempSprite(sf::Color::Red, 16, 16);
	tileMap.LoadFromFile("tilemap5.txt");
	tileMap.SaveToFile("tilemapsave.txt");
	path = new Path(tileMap);
}

Game::~Game()
{
}

void Game::Run()
{
	while (window.isOpen())
	{
		ProcessEvents();
		frameWait -= 1;
		if (frameWait <= 0)
		{
			frameWait = 600;
			if (currentNode >= path->nodePoints.size())
			{
				currentNode = 0;
			}

			enemySprite->setPosition(path->nodePoints[currentNode].x * 32 + 8, path->nodePoints[currentNode].y * 32 + 8);
			currentNode++;
			
		}
		Render();
	}
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
	}
}

void Game::Render()
{
	window.clear(sf::Color::Magenta);
	tileMap.Render(&window);
	window.draw(*enemySprite);
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
