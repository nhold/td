#ifndef ENEMYSPAWNER_HPP
#define ENEMYSPAWNER_HPP

#include <enemy.hpp>
#include <spawner.hpp>

namespace sf
{
	class RenderWindow;
}

class EnemySpawner : public Spawner<Enemy>
{
public:
	std::vector<Enemy*> deadEnemyVector;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	std::function<void(Enemy*)> EnemyKilled;
private:
	
	void DestroyDead();
};

#endif // !ENEMYSPAWNER_HPP
