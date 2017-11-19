#include <projectile.hpp>
#include <path.hpp>
#include <vectorutility.hpp>
#include <game.hpp>
#include <iostream>
#include <enemy.hpp>
#include <SFML/Graphics/Sprite.hpp>

Projectile::Projectile()
{
	movementSpeed = 1;
	damage = 1;
}

Projectile::Projectile(int movementSpeed, int damage, sf::Sprite* sprite, Enemy* enemy, std::string name)
{
	node.SetSprite(sprite);
	node.SetName(name);
	node.SetOrigin(0.5f, 0.5f);
	this->movementSpeed = movementSpeed;
	this->damage = damage;
}

Projectile::Projectile(const Projectile& otherProjectile) : node(otherProjectile.node)
{

	movementSpeed = otherProjectile.movementSpeed;
	damage = otherProjectile.damage;

	node.SetOrigin(0.5f, 0.5f);
}

Projectile::~Projectile()
{

}

void Projectile::Update()
{
	if (!enemy->node.isAlive)
	{
		enemy = nullptr;
		node.isAlive = false;
		return;
	}

	if (!AtTarget())
	{
		Move();
	}
	else
	{
		node.isAlive = false;
		enemy->currentHealth -= damage;
	}
}

bool Projectile::AtTarget()
{
	sf::Vector2f direction = enemy->node.GetPosition() - node.GetPosition();

	if (Magnitude(direction) <= movementSpeed * Game::deltaTime)
	{
		return true;
	}

	return false;
}


bool Projectile::Move()
{
	sf::Vector2f floatVersion;

	auto currentNodeWorldPosition = enemy->node.GetPosition();

	sf::Vector2f direction = currentNodeWorldPosition - node.GetPosition();
	direction = Normalise(direction);
	node.SetDirection(direction);
	direction = Scale(direction, movementSpeed * Game::deltaTime);

	node.SetPosition(node.GetPosition() + direction);

	return true;
}
