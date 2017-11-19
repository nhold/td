#include <tower.hpp>
#include <game.hpp>
#include <enemy.hpp>
#include <iostream>
#include <vectorutility.hpp>
#include <node.hpp>

Tower::Tower(int aDamage, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* sprite, std::string name, Spawner<Projectile>& projectileSpawner) : projectileSpawner(projectileSpawner)
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
}

Tower::Tower(const Tower & otherTower) : node(otherTower.node), projectileSpawner(otherTower.projectileSpawner)
{
	damage = otherTower.damage;
	numberOfTargets = otherTower.numberOfTargets;
	this->buildTime = otherTower.buildTime;
	this->radius = otherTower.radius;
	this->fireRate = otherTower.fireRate;
	this->cost = otherTower.cost;
	currentRate = fireRate;

	targets = otherTower.targets;
}

Tower::~Tower()
{
}


void Tower::Update(std::vector<Enemy*>& allEnemies)
{
	
	if (isBuilding)
	{
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

sf::VertexArray Tower::GetDebugLines()
{
	sf::VertexArray vertexArray(sf::Lines, 0);
	for each (auto enemy in targets)
	{
		sf::Vertex vert;
		vert.color = sf::Color::Green;
		vert.position = node.GetPosition();
		vertexArray.append(vert);
		sf::Vertex vert2;
		vert2.color = sf::Color::Red;
		vert2.position = enemy->node.GetPosition();
		vertexArray.append(vert2);
	}
	return vertexArray;
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
			if (Magnitude(vec) > radius || !enemy->node.isAlive)
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

	if (targets.size() == 0)
		currentRate = fireRate;

}

void Tower::FindTarget(std::vector<Enemy*>& allEnemies)
{
	for (auto it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		auto vec = (*it)->node.GetSprite()->getPosition() - node.GetSprite()->getPosition();
		if (Magnitude(vec) <= radius)
		{
			if (std::find(targets.begin(), targets.end(), (*it)) == targets.end())
			{
				targets.push_back((*it));
			}
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
			auto projectile = projectileSpawner.Spawn(damage);
			projectile->enemy = enemy;
			projectile->node.SetPosition(node.GetPosition());
		}
	}
}
