#include <td.hpp>
#include <path.hpp>
#include <enemy.hpp>
#include <base.hpp>

Td::Td() : Game(640, 640, "TD"), distribution(0, 1), currentLevel(enemySpawner)
{

}

Td::~Td()
{
	while (towers.size() > 0)
	{
		auto tower = towers.back();
		delete tower;
		towers.pop_back();
	}
}

void Td::Initialise()
{
	goldText.setFont(debugFont);
	goldText.setCharacterSize(14);
	goldText.setFillColor(sf::Color::Blue);

	currentLevel.Load("assets/level1_tilemap.txt", assetDatabase);

	CreateTypes();

	cursor.setTexture(assetDatabase.GetTexture("assets/cursor.png"));
	cursor.setPosition(0, 0);
	cursor.setOrigin(16, 16);

	towerRadius.setRadius(towerTypes[0].radius);
	towerRadius.setOrigin(towerTypes[0].radius, towerTypes[0].radius);
	towerRadius.setFillColor(sf::Color::Transparent);
	towerRadius.setOutlineColor(sf::Color::Black);
	towerRadius.setOutlineThickness(2.f);
}

void Td::Update()
{
	Game::Update();
	currentLevel.Update(enemySpawner.enemyInstances, towers);

	auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	auto worldGridMousePosition = Game::WorldToGrid(sf::Vector2f(sf::Mouse::getPosition(window)));

	if (mousePosition.x >= 0 || mousePosition.y >= 0)
	{
		auto grid = Game::WorldToArray(mousePosition);

		if (currentLevel.tileMap.tiles[grid.x][grid.y] == 1 || currentLevel.buildingMap.isBlocked[grid.x][grid.y])
		{
			cursor.setColor(sf::Color::Red);
		}
		else
		{
			cursor.setColor(sf::Color::Black);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (towerTypes[0].cost <= currentLevel.currentGold)
				{
					currentLevel.buildingMap.isBlocked[grid.x][grid.y] = true;
					Tower* tower = new Tower(towerTypes[0]);
					towerRadius.setRadius(towerTypes[0].radius);
					towerRadius.setOrigin(towerTypes[0].radius, towerTypes[0].radius);
					tower->isBuilding = true;
					tower->node.SetPosition(worldGridMousePosition);
					towers.push_back(tower);
					currentLevel.currentGold -= towerTypes[0].cost;
					goldText.setString("Gold: " + std::to_string(currentLevel.currentGold));
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

	goldText.setString("Gold: " + std::to_string(currentLevel.currentGold) + "\n" + "Health: " + std::to_string(currentLevel.base->health));
}

void Td::Render()
{
	currentLevel.Render(window);

	for (auto it = towers.begin(); it != towers.end(); ++it)
	{
		window.draw(*(*it)->node.GetSprite());
		window.draw((*it)->GetDebugLines());
	}

	for (auto it = enemySpawner.enemyInstances.begin(); it != enemySpawner.enemyInstances.end(); ++it)
	{
		window.draw(*(*it)->node.GetSprite());
	}

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
			enemySpawner.Spawn(dice_roll);
		}

		if (currentEvent.key.code == sf::Keyboard::Space)
		{
			enemySpawner.Spawn(2);
		}

		if (currentEvent.key.code == sf::Keyboard::D)
		{
			enemySpawner.DespawnBack();
		}
	}
}

void Td::CreateTypes()
{
	Enemy enemy1(10, 50, 5, 5, new sf::Sprite(assetDatabase.GetTexture("assets/enemy1.png")), currentLevel.path, "Simpleton");
	enemy1.node.SetPosition(GridToWorld(currentLevel.path->nodePoints[0]));
	enemy1.node.SetFont(debugFont);

	Enemy enemy2(5, 100, 5, 10, new sf::Sprite(assetDatabase.GetTexture("assets/enemy2.png")), currentLevel.path, "Blarg");
	enemy2.node.SetPosition(GridToWorld(currentLevel.path->nodePoints[0]));
	enemy2.node.SetFont(debugFont);

	Enemy enemy3(1000, 25, 50, 120, new sf::Sprite(assetDatabase.GetTexture("assets/demon.png")), currentLevel.path, "Demon");
	enemy3.node.SetPosition(GridToWorld(currentLevel.path->nodePoints[0]));
	enemy3.node.SetFont(debugFont);

	enemySpawner.AddType(enemy1);
	enemySpawner.AddType(enemy2);
	enemySpawner.AddType(enemy3);

	Tower tower1(2, 3, 5.f, 100.f, 1.f, 25, new sf::Sprite(assetDatabase.GetTexture("assets/tower1.png")), "Tower One");
	tower1.node.SetFont(debugFont);
	towerTypes.push_back(tower1);
}