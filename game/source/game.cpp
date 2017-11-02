#include "..\include\game.hpp"

Game::Game()
{
	window.create(sf::VideoMode(640, 480), "TD");

	tileMap.tileTypes[0] = CreateTempSprite(sf::Color::Green);
	tileMap.tileTypes[1] = CreateTempSprite(sf::Color::Black);
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);

	tileMap.LoadFromFile("tilemap.txt");
	tileMap.SaveToFile("tilemapsave.txt");
}

Game::~Game()
{
}

void Game::Run()
{
	while (window.isOpen())
	{
		ProcessEvents();
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
	window.display();
}

// TODO: Extract to an asset helper class.
sf::Sprite* Game::CreateTempSprite(const sf::Color &color) const
{
	sf::Texture texture;
	texture.create(32, 32);
	sf::Image image;
	image.create(32, 32, color);
	texture.update(image, 0, 0);

	auto sprite = new sf::Sprite(texture);
	sprite->setColor(color);
	
	return sprite;
}
