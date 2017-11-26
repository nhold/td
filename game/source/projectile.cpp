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
	enemy = nullptr;
	cacheEnemyPosition = false;
}

Projectile::Projectile(int movementSpeed, int damage, sf::Sprite* sprite, bool cacheEnemyPosition, float radius, std::string name)
{
	node.SetSprite(sprite);
	node.SetName(name);
	node.SetOrigin(0.5f, 0.5f);
	this->enemy = enemy;
	this->movementSpeed = movementSpeed;
	this->damage = damage;
	this->cacheEnemyPosition = cacheEnemyPosition;
	this->radius = radius;
}

Projectile::Projectile(const Projectile& otherProjectile) : node(otherProjectile.node)
{
	movementSpeed = otherProjectile.movementSpeed;
	damage = otherProjectile.damage;
	this->cacheEnemyPosition = otherProjectile.cacheEnemyPosition;
	this->radius = otherProjectile.radius;
	node.SetOrigin(0.5f, 0.5f);
}

Projectile::~Projectile()
{

}

void Projectile::Update(Spawner<Enemy>& enemySpawner)
{
	if (enemy == nullptr)
		return;

	// If we are following the enemy and it goes away we should kill ourselves
	if (!enemy->node.isAlive && !cacheEnemyPosition)
	{
		enemy = nullptr;
		node.isAlive = false;
		return;
	}

	if (!AtTarget(GetCurrentTargetPosition()))
	{
		Move(GetCurrentTargetPosition());
	}
	else
	{
		node.isAlive = false;
		if (!cacheEnemyPosition)
		{
			enemy->currentHealth -= damage;
		}

		if (radius > 0)
		{
			auto enemies = enemySpawner.InArea(node.GetPosition(), radius);
			for each (auto enemy in enemies)
			{
				enemy->currentHealth -= damage;
			}
		}
	}
	
}

void Projectile::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;
	if (cacheEnemyPosition)
	{
		targetPosition = enemy->node.GetPosition();
	}
}


bool Projectile::AtTarget(sf::Vector2f otherPosition)
{
	sf::Vector2f direction = otherPosition - node.GetPosition();
	//sf::Vector2f direction = enemy->node.GetPosition() - node.GetPosition();

	if (Magnitude(direction) <= movementSpeed * Game::deltaTime)
	{
		return true;
	}

	return false;
}

sf::Vector2f Projectile::GetCurrentTargetPosition()
{
	return cacheEnemyPosition ? targetPosition : enemy->node.GetPosition();
}


bool Projectile::Move(sf::Vector2f otherPosition)
{
	sf::Vector2f direction = otherPosition - node.GetPosition();
	direction = Normalise(direction);
	node.SetDirection(direction);
	direction = Scale(direction, movementSpeed * Game::deltaTime);

	node.SetPosition(node.GetPosition() + direction);

	return true;
}
