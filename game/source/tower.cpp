#include <tower.hpp>
#include <game.hpp>
#include <enemy.hpp>
#include <iostream>
#include <vectorutility.hpp>

Tower::Tower()
{
	damage = 1;
	buildTime = 1;
	numberOfTargets = 1;
	radius = 1.f;
	fireRate = 1.f;
	cost = 1;
	currentRate = fireRate;
	sprite = nullptr;
	name = "null";
	text.setString(name);
	text.setColor(sf::Color::Magenta);
	isBuilding = false;
	std::cout << text.getString().toAnsiString() << " Default Constructor." << std::endl;
}

Tower::Tower(int aDamage, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* aSprite, std::string aName)
{
	damage = aDamage;
	numberOfTargets = allowedTargetCount;
	this->buildTime = buildTime;
	this->radius = radius;
	this->fireRate = fireRate;
	this->cost = cost;
	currentRate = fireRate;
	SetSprite(aSprite);
	name = aName;
	text.setString(name);
	text.setColor(sf::Color::Magenta);
	isBuilding = false;
	std::cout << text.getString().toAnsiString() << " Data Constructor. " << std::endl;
}

Tower::Tower(const Tower & otherTower)
{
	damage = otherTower.damage;
	numberOfTargets = otherTower.numberOfTargets;
	this->buildTime = otherTower.buildTime;
	this->radius = otherTower.radius;
	this->fireRate = otherTower.fireRate;
	this->cost = otherTower.cost;
	currentRate = fireRate;
	if (otherTower.sprite != nullptr)
	{
		sprite = new sf::Sprite(*otherTower.sprite);
		SetPosition(otherTower.sprite->getPosition());
	}

	targets = otherTower.targets;

	name = otherTower.name;
	text.setString(name);
	text.setColor(sf::Color::Magenta);


	if (otherTower.text.getFont() != nullptr)
	{
		text.setFont(*otherTower.text.getFont());
		text.setCharacterSize(12);
	}

	std::cout << text.getString().toAnsiString() << " Copy Constructor" << std::endl;
}

Tower::~Tower()
{
	if (sprite != nullptr)
		delete sprite;
}

void Tower::SetSprite(sf::Sprite * aSprite)
{
	sprite = new sf::Sprite(*aSprite);
	sprite->setOrigin(sprite->getTextureRect().width*0.5f, sprite->getTextureRect().height*0.5f);
}

void Tower::SetPosition(float x, float y)
{
	sprite->setPosition(x, y);
	text.setPosition(x, y + 20);
}

void Tower::SetPosition(sf::Vector2f newPosition)
{
	sprite->setPosition(newPosition);
	text.setPosition(newPosition + sf::Vector2f(0, 20));
}

sf::Sprite * Tower::GetSprite()
{
	return sprite;
}

sf::Text & Tower::GetText()
{
	return text;
}

std::string Tower::GetName()
{
	return name;
}

void Tower::SetFont(const sf::Font & font)
{
	text.setFont(font);
	text.setCharacterSize(12);
}

void Tower::SetName(std::string aName)
{
	name = aName;
}

void Tower::Update(std::vector<Enemy*>& allEnemies)
{
	
	if (isBuilding)
	{
		text.setString(name + " : Building...");
		buildTime -= Game::deltaTime;
		if (buildTime <= 0)
		{
			isBuilding = false;
		}
		else
		{
			return;
		}
	}

	text.setString(name + " : Built : Targets: " + std::to_string(targets.size()));
	RemoveDeadTargets(allEnemies);

	if (targets.size() < numberOfTargets)
	{
		FindTarget(allEnemies);
	}

	if (targets.size() > 0)
	{
		Shoot();
	}
}

void Tower::RemoveDeadTargets(std::vector<Enemy*>& allEnemies)
{
	std::vector<Enemy*> removeVec;
	for each (auto enemy in targets)
	{
		auto it = std::find(allEnemies.begin(), allEnemies.end(), enemy);
		if (it == allEnemies.end())
		{
			removeVec.push_back(enemy);
		}
		else
		{
			auto vec = enemy->GetSprite()->getPosition() - sprite->getPosition();
			if (Magnitude(vec) > radius)
			{
				removeVec.push_back(enemy);
			}
		}
	}

	for each (auto enemy in removeVec)
	{
		auto it = std::find(targets.begin(), targets.end(), enemy);
		if (it != targets.end())
			targets.erase(it);
	}
}

void Tower::FindTarget(std::vector<Enemy*>& allEnemies)
{
	for (auto it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		auto vec = (*it)->GetSprite()->getPosition() - sprite->getPosition();
		if (Magnitude(vec) <= radius)
		{
			targets.push_back((*it));
		}

		if (targets.size() >= numberOfTargets)
			return;
	}
}

void Tower::Shoot()
{
	currentRate -= Game::deltaTime;
	if (currentRate <= 0)
	{
		currentRate = fireRate;
		for each (auto enemy in targets)
		{
			enemy->currentHealth -= damage;
		}
	}
}
