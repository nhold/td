#include <td.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <base.hpp>

Td::Td() : Game(640, 640, "TD"), distribution(0, 1)
{
	gold = 100;

	tileMap.tileTypes[0] = CreateTempSprite(sf::Color::Green);
	tileMap.tileTypes[1] = CreateTempSprite(sf::Color(125, 68, 29));
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);

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

	towerRadius.setRadius(1);
	towerRadius.setFillColor(sf::Color::Transparent);
	towerRadius.setOutlineColor(sf::Color::Black);
	towerRadius.setOutlineThickness(2.f);
}

Td::~Td()
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

	delete base;
}

void Td::Initialise()
{
	gold = 100; // TODO: Load from level file.
	goldText.setString("Gold: " + std::to_string(gold));
}

void Td::Update()
{
	Game::Update();

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

	auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	auto worldGridMousePosition = WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window)));

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
					towerRadius.setRadius(towerTypes[0].radius);
					towerRadius.setOrigin(towerTypes[0].radius, towerTypes[0].radius);
					tower->isBuilding = true;
					tower->node.SetPosition(worldGridMousePosition);
					towers.push_back(tower);
					gold -= towerTypes[0].cost;
					goldText.setString("Gold: " + std::to_string(gold));
				}
				else
				{
					cursor.setColor(sf::Color::Blue);
				}
			}
		}

		cursor.setPosition(WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window))));
		towerRadius.setPosition(worldGridMousePosition);
	}
}

void Td::Render()
{
	tileMap.Render(&window);

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		window.draw(*(*it)->node.GetSprite());
	}

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		window.draw(*(*it)->node.GetSprite());
	}

	window.draw(*base->node.GetSprite());
	window.draw(cursor);
	window.draw(towerRadius);
	window.draw(goldText);
}

void Td::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::KeyPressed)
	{
		if (currentEvent.key.code == sf::Keyboard::A)
		{
			int dice_roll = distribution(generator);
			auto eT = enemyTypes[dice_roll];
			eT.node.SetName(eT.node.GetName() + " --ID:-- " + std::to_string(enemies.size()));
			enemies.push_back(new Enemy(eT));
		}

		if (currentEvent.key.code == sf::Keyboard::Space)
		{
			auto eT = new Enemy(enemyTypes[2]);
			eT->node.SetName(eT->node.GetName() + " --ID:-- " + std::to_string(enemies.size()));
			enemies.push_back(eT);
		}

		if (currentEvent.key.code == sf::Keyboard::D)
		{
			if (enemies.size() > 0)
			{
				auto enemy = enemies.back();
				enemies.pop_back();
				delete enemy;
			}
		}

		if (currentEvent.key.code == sf::Keyboard::C)
		{
			///debugEntities = false;
		}

		if (currentEvent.key.code == sf::Keyboard::Z)
		{
			//debugEntities = true;
		}
	}
}

void Td::CreateTypes()
{
	Enemy enemy1(10, 50, 5, 5, new sf::Sprite(GetTexture("assets/enemy1.png")), path, "Simpleton");
	enemy1.node.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy1.node.SetFont(debugFont);

	Enemy enemy2(10, 100, 5, 10, new sf::Sprite(GetTexture("assets/enemy2.png")), path, "Blarg");
	enemy2.node.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy2.node.SetFont(debugFont);

	Enemy enemy3(1000, 25, 50, 120, new sf::Sprite(GetTexture("assets/demon.png")), path, "Demon");
	enemy3.node.SetPosition(GridToWorld(path->nodePoints[0]));
	enemy3.node.SetFont(debugFont);

	enemyTypes.push_back(enemy1);
	enemyTypes.push_back(enemy2);
	enemyTypes.push_back(enemy3);

	Tower tower1(2, 1, 5.f, 100.f, 1.f, 25, new sf::Sprite(GetTexture("assets/tower1.png")), "Tower One");
	tower1.node.SetFont(debugFont);
	towerTypes.push_back(tower1);

	base = new Base(100, new sf::Sprite(GetTexture("assets/base.png")), "Base");
	base->node.SetPosition(GridToWorld(path->nodePoints.back()));
}
