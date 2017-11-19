#include <enemyspawner.hpp>

EnemySpawner::EnemySpawner()
{
}

EnemySpawner::~EnemySpawner()
{
	while (enemyInstances.size() > 0)
	{
		DespawnBack();
	}
}

void EnemySpawner::AddType(Enemy type)
{
	enemyTypes.push_back(type);
}

void EnemySpawner::Spawn(int typeID)
{
	if (typeID < enemyTypes.size())
	{
		auto eT = new Enemy(enemyTypes[typeID]);
		eT->node.SetName(eT->node.GetName() + " --ID:-- " + std::to_string(enemyInstances.size()));
		enemyInstances.push_back(eT);
	}
}

void EnemySpawner::DespawnBack()
{
	if (enemyInstances.size() > 0)
	{
		auto enemy = enemyInstances.back();
		enemyInstances.pop_back();
		delete enemy;
	}
}
