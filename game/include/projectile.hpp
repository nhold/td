#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <node.hpp>

class Enemy;

namespace sf 
{
	class Sprite;
}

class Projectile
{
public:
	Projectile();
	Projectile(int movementSpeed, int damage, sf::Sprite* sprite, Enemy* enemy, std::string name);
	Projectile(const Projectile& otherProjectile);
	~Projectile();

	int movementSpeed;
	int damage;

	Node node;

	void Update();
	Enemy* enemy;
	sf::Vector2i targetPosition;
private:
	bool Move();
	bool AtTarget();

	
};

#endif
