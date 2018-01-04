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
	Projectile(int movementSpeed, int damage, sf::Sprite* sprite, bool cacheEnemyPosition, float radius, int bounceCount, float bounceRadius, std::string name, Spawner<Projectile>& projectileSpawner);

	Projectile(const Projectile& otherProjectile);
	~Projectile();

	int movementSpeed;
	int damage;

	Node node;

	void Update(Spawner<Enemy>& enemySpawner, float deltaTime);

	void SetEnemy(Enemy* enemy);
	void SetBounceCount(int bounceCount);
private:
	void Bounce(Spawner<Enemy>& enemySpawner);
	Spawner<Projectile>& projectileSpawner;
	int bounceCount;
	bool bounced;
	float radius;
	float bounceRadius;
	bool Move(sf::Vector2f otherPosition, float deltaTime);
	bool AtTarget(sf::Vector2f otherPosition);
	sf::Vector2f GetCurrentTargetPosition();

	Enemy* enemy;
	sf::Vector2f targetPosition;
	bool cacheEnemyPosition;

	
};

#endif
