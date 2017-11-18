#include <game.hpp>

#include <iostream>

#include <path.hpp>
#include <enemy.hpp>
#include <vectorutility.hpp>
#include <base.hpp>



float Game::deltaTime = 0.f;

Game::Game()
{
	window.create(sf::VideoMode(640, 640), "Default Window Title");
	debugFont.loadFromFile("assets/Consolas.ttf");
}

Game::Game(int width, int height, std::string title)
{
	window.create(sf::VideoMode(width, height), title);
	debugFont.loadFromFile("assets/Consolas.ttf");
}

Game::~Game()
{
}

void Game::Update()
{
	if (showDebugText)
	{
		debugText.setString("\n\n\n\n\nDelta time: " + std::to_string(deltaTime) +
			"\n" + "FPS: " + std::to_string(1.f / deltaTime) +
			"\n" + "Mouse Pos: " + ToString(sf::Mouse::getPosition()) +
			"\n" + "Mouse Pressed: " + std::to_string((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))));
	}
}

void Game::Run()
{
	Initialise();

	while (window.isOpen())
	{
		ProcessEvents();

		Update();

		window.clear(sf::Color::Magenta);
		Render();

		if(showDebugText)
			window.draw(debugText);

		window.display();

		deltaTime = clock.restart().asSeconds();
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

		ProcessInput(event);
		
	}
}

void Game::Initialise()
{
}

void Game::Render()
{
}

void Game::ProcessInput(sf::Event currentEvent)
{
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
