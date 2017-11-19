#include <tilemap.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

TileMap::TileMap()
{
	for (int x = 0; x < tileMapSize; x++)
	{
		for (int y = 0; y < tileMapSize; y++)
		{
			tiles[x][y] = 0;

			if ((x + y) % 2 == 0)
			{
				tiles[x][y] = 1;
			}
		}
	}
}

TileMap::~TileMap()
{
}

void TileMap::Render(sf::RenderWindow* window)
{
	for (int x = 0; x < tileMapSize; x++)
	{
		for (int y = 0; y < tileMapSize; y++)
		{
			//if (tiles[x][y] < 2)
			//{
				sf::Sprite* sprite = tileTypes[tiles[x][y]];
				sprite->setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
				window->draw(*sprite);
			//}
		}
	}
}

void TileMap::LoadFromFile(const std::string& fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{
		int x = 0;
		int y = 0;

		while (getline(myfile, line))
		{
			for (int i = 0; i < line.length(); i++)
			{
				/*if (line[i] == '\n')
					break;*/

				if (x >= tileMapSize)
				{
					std::cout << "X is out of bounds, too many characters in line: " + std::to_string(i) << std::endl;
					break;
				}

				tiles[x][y] = line[i] - '0';


				x++;
			}
			x = 0;
			y++;

			if (y >= tileMapSize)
			{
				std::cout << "Y is out of bounds, too many lines in file." << std::endl;
				break;
			}
		}
		myfile.close();
	}
}

bool TileMap::WithinBounds(sf::Vector2i point)
{
	return (point.x >= 0 && point.x < tileMapSize
		&& point.y >= 0 && point.y < tileMapSize);
}

void TileMap::SaveToFile(const std::string& fileName)
{
	std::string line;
	std::ofstream myfile(fileName);

	if (myfile.is_open())
	{
		for (int y = 0; y < tileMapSize; y++)
		{
			for (int x = 0; x < tileMapSize; x++)
			{
				line += std::to_string(tiles[x][y]);
			}
			line += '\n';
		}

		myfile << line;

		myfile.close();
	}
}

