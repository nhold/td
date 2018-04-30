#include <tower.hpp>
#include <game.hpp>
#include <enemy.hpp>
#include <iostream>
#include <vectorutility.hpp>
#include <node.hpp>


Tower::Tower(int projectileID, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* sprite, std::string name, Spawner<Projectile>& projectileSpawner) : projectileSpawner(projectileSpawner)
{
	this->projectileID = projectileID;
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
	this->projectileID = otherTower.projectileID;
	this->numberOfTargets = otherTower.numberOfTargets;
	this->buildTime = otherTower.buildTime;
	this->radius = otherTower.radius;
	this->fireRate = otherTower.fireRate;
	this->cost = otherTower.cost;
	this->isBuilding = otherTower.isBuilding;

	currentRate = fireRate;
	targets = otherTower.targets;
}

Tower::~Tower()
{
}

void Tower::Update(EnemySpawner& enemySpawner, float deltaTime)
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

	RemoveDeadTargets(enemySpawner);

	if (targets.size() < numberOfTargets)
	{
		FindTarget(enemySpawner);
	}

	if (targets.size() > 0)
	{
		Shoot(deltaTime);
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

void Tower::RemoveDeadTargets(EnemySpawner& enemySpawner)
{
	std::vector<Enemy*> removeVec;
	for each (auto enemy in targets)
	{
		auto it = std::find(enemySpawner.instances.begin(), enemySpawner.instances.end(), enemy);
		if (it == enemySpawner.instances.end())
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

void Tower::FindTarget(EnemySpawner& enemiesSpawner)
{
	if (targets.size() >= numberOfTargets)
		return;

	float distance = 99999;
	Enemy* newTarget = nullptr;

	for (auto it = enemiesSpawner.instances.begin(); it != enemiesSpawner.instances.end(); ++it)
	{
		auto enemy = (*it);
		auto vec = enemy->node.GetSprite()->getPosition() - node.GetSprite()->getPosition();

		float newClosestDistance = Magnitude(vec);
		float old = distance;

		if (std::find(targets.begin(), targets.end(), enemy) == targets.end())
		{
			if (newClosestDistance <= radius)
			{
				if (newClosestDistance < distance)
				{
					distance = newClosestDistance;
					newTarget = enemy;
				}
			}
		}
	}

	if (newTarget != nullptr)
	{
		targets.push_back(newTarget);
	}
}

void Tower::Shoot(float deltaTime)
{
	currentRate -= deltaTime;
	if (currentRate <= 0)
	{
		currentRate = fireRate;
		for each (auto enemy in targets)
		{
			auto projectile = projectileSpawner.Spawn(projectileID);
			projectile->SetEnemy(enemy);
			projectile->node.SetPosition(node.GetPosition());
		}
	}
}
