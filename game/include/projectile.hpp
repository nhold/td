#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <node.hpp>
#include <spawner.hpp>

class Enemy;

namespace sf 
{
	class Sprite;
}

class Projectile
{
public:
	Projectile();
	Projectile(int movementSpeed, int damage, sf::Sprite* sprite, bool cacheEnemyPosition, float radius, std::string name);

	Projectile(const Projectile& otherProjectile);
	~Projectile();

	int movementSpeed;
	int damage;

	Node node;

	void Update(Spawner<Enemy>& enemySpawner);

	void SetEnemy(Enemy* enemy);

private:
	int bounceCount;
	float radius;
	bool Move(sf::Vector2f otherPosition);
	bool AtTarget(sf::Vector2f otherPosition);
	sf::Vector2f GetCurrentTargetPosition();

	Enemy* enemy;
	sf::Vector2f targetPosition;
	bool cacheEnemyPosition;

	
};

#endif
