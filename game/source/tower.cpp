#include <tower.hpp>
#include <game.hpp>
#include <enemy.hpp>
#include <iostream>
#include <vectorutility.hpp>
#include <node.hpp>

Tower::Tower()
{
	damage = 1;
	buildTime = 1;
	numberOfTargets = 1;
	radius = 1.f;
	fireRate = 1.f;
	cost = 1;
	currentRate = fireRate;
	isBuilding = false;
	std::cout << node.GetText().getString().toAnsiString() << " Default Constructor." << std::endl;
}

Tower::Tower(int aDamage, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* sprite, std::string name)
{
	damage = aDamage;
	node.SetSprite(sprite);
	node.SetName(name);
	numberOfTargets = allowedTargetCount;
	this->buildTime = buildTime;
	this->radius = radius;
	this->fireRate = fireRate;
	this->cost = cost;
	currentRate = fireRate;
	
	isBuilding = false;
	//std::cout << text.getString().toAnsiString() << " Data Constructor. " << std::endl;
}

Tower::Tower(const Tower & otherTower) : node(otherTower.node)
{
	damage = otherTower.damage;
	numberOfTargets = otherTower.numberOfTargets;
	this->buildTime = otherTower.buildTime;
	this->radius = otherTower.radius;
	this->fireRate = otherTower.fireRate;
	this->cost = otherTower.cost;
	currentRate = fireRate;


	targets = otherTower.targets;



	std::cout << node.GetText().getString().toAnsiString() << " Copy Constructor" << std::endl;
}

Tower::~Tower()
{
}


void Tower::Update(std::vector<Enemy*>& allEnemies)
{
	
	if (isBuilding)
	{
		//text.setString(name + " : Building...");
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

	//text.setString(name + " : Built : Targets: " + std::to_string(targets.size()));
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
			auto vec = enemy->node.GetSprite()->getPosition() - node.GetSprite()->getPosition();
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
		auto vec = (*it)->node.GetSprite()->getPosition() - node.GetSprite()->getPosition();
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
