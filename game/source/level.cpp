#include <level.hpp>

#include <path.hpp>
#include <assethelper.hpp>
#include <game.hpp>
#include <base.hpp>
#include <vector>
#include <enemy.hpp>
#include <tower.hpp>
#include <assetdatabase.hpp>
#include <fstream>

Level::Level(AssetDatabase& assetDatabase) : assetDatabase(assetDatabase)
{
	path = nullptr;
	startingGold = 100;
}

Level::~Level()
{
	if (path != nullptr)
		delete path;
}

void Level::Load(std::string filename)
{
	// Grab the level metadata.
	std::string line;
	std::ifstream myfile(filename);

	if (myfile.is_open())
	{
		int i = 0;
		
		while (getline(myfile, line))
		{
			auto splitIndex = line.find_first_of(":");
			auto firstString = line.substr(0, splitIndex);
			auto secondString = line.substr(splitIndex + 1);

			if (firstString == "name")
			{
				name = secondString;
			}

			if (firstString == "tilemap")
			{
				tileMap.LoadFromFile(secondString);
			}

			if (firstString == "startinggold")
			{
				startingGold = std::stoi(secondString);
			}

			if (firstString == "wavedata")
			{
				Wave wave;
				wave.LoadFromFile(secondString);
				waves.push_back(wave);
			}

			if (firstString == "end")
				break;
		}

		path = new Path(tileMap);
		base = new Base(100, new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/base.png").resource), "Base");
		base->node.SetPosition(Game::GridToWorld(path->nodePoints.back()));

		myfile.close();
	}
}

void Level::Render(sf::RenderWindow & window)
{
	tileMap.Render(&window);
	window.draw(*base->node.GetSprite());
}

bool Level::isValid()
{
	return (base != nullptr);
}

void Level::Clear()
{
	buildingMap.Clear();
	waves.clear();
}
