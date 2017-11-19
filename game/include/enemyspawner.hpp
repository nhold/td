#ifndef ENEMYSPAWNER_HPP
#define ENEMYSPAWNER_HPP

#include <enemy.hpp>

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();
	void AddType(Enemy type);
	void Spawn(int typeID);
	void DespawnBack();

	std::vector<Enemy> enemyTypes;
	std::vector<Enemy*> enemyInstances;
};

#endif // !ENEMYSPAWNER_HPP
