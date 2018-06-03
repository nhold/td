#include <enemyspawner.hpp>

void EnemySpawner::Update(float deltaTime)
{
	DestroyDead();

	mutex.lock();
	for (auto it = instances.begin(); it != instances.end(); ++it)
	{
		auto enemy = (*it);
		enemy->Update(deltaTime);

		if (!enemy->node.isAlive)
		{
			deadEnemyVector.push_back(enemy);
		}
	}
	mutex.unlock();
}

void EnemySpawner::Render(sf::RenderWindow& window)
{
	mutex.lock();
	for (auto it = instances.begin(); it != instances.end(); ++it)
	{
		window.draw(*(*it)->node.GetSprite());

		(*it)->RenderHealthbars(window);
	}
	mutex.unlock();
}

void EnemySpawner::DestroyDead()
{
	mutex.lock();
	for (auto it = deadEnemyVector.begin(); it != deadEnemyVector.end(); ++it)
	{
		auto killIt = std::find(instances.begin(), instances.end(), (*it));

		if (killIt != instances.end())
		{
			EnemyKilled((*it));
			instances.erase(killIt);
			delete (*it);
		}
	}

	deadEnemyVector.clear();
	mutex.unlock();
}
