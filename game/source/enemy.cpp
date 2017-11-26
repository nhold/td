#include <enemy.hpp>
#include <path.hpp>
#include <vectorutility.hpp>
#include <game.hpp>
#include <iostream>
#include <assethelper.hpp>
#include <mathutility.hpp>

Enemy::Enemy()
{
	currentHealth = 1;
	maximumHealth = 1;
	movementSpeed = 1;
	damage = 1;
	worth = 1;
	currentNode = 0;
	healthBarSprite = CreateTempSprite(sf::Color::Green, 32, 8);
	healthBarSprite->setOrigin(16, 4);
}

Enemy::Enemy(int health, int movementSpeed, int damage, int worth, sf::Sprite* sprite, Path* path, std::string name)
{
	node.SetSprite(sprite);
	node.SetName(name);
	node.SetOrigin(0.5f, 0.9f);
	currentHealth = health;
	maximumHealth = currentHealth;
	this->movementSpeed = movementSpeed;
	this->damage = damage;
	SetPath(path);
	this->worth = worth;
	currentNode = 0;
	healthBarSprite = CreateTempSprite(sf::Color::Green, 32, 8);
	healthBarSprite->setOrigin(16, 4);
}

Enemy::Enemy(const Enemy & otherEnemy) : node(otherEnemy.node)
{
	currentHealth = otherEnemy.currentHealth;
	maximumHealth = otherEnemy.maximumHealth;
	movementSpeed = otherEnemy.movementSpeed;
	damage = otherEnemy.damage;
	worth = otherEnemy.worth;
	nodePoints = otherEnemy.nodePoints;
	currentNode = otherEnemy.currentNode;
	healthBarSprite = CreateTempSprite(sf::Color::Green, 32, 8);
	healthBarSprite->setOrigin(16, 4);
	node.SetOrigin(0.5f, 0.9f);
}

Enemy::~Enemy()
{
	delete healthBarSprite;
}

void Enemy::SetPath(Path* path)
{
	if (path == nullptr)
		return;

	nodePoints = path->nodePoints;
	currentNode = 0;
}

void Enemy::Update()
{
	if (currentHealth <= 0)
	{
		node.isAlive = false;
	}

	if (nodePoints.size() == 0)
		return;

	if (currentNode < nodePoints.size())
	{
		MoveToCurrentNode();

		if (AtCurrentNode())
			currentNode++;
	}
	else 
	{
		currentNode = 0;
		node.SetPosition(Game::GridToWorld(nodePoints[currentNode]));
		std::cout << node.GetText().getString().toAnsiString() << std::endl;
	}
}

void Enemy::RenderHealthbars(sf::RenderWindow & window)
{
	healthBarSprite->setPosition(node.GetPosition() + sf::Vector2f(0.0f, -32.f));
	healthBarSprite->setScale(MapRange(0, maximumHealth, currentHealth, 0.0f, 1.0f), 1.0f);

	window.draw(*healthBarSprite);
}

bool Enemy::AtCurrentNode()
{
	sf::Vector2f worldPosition;
	worldPosition = Game::GridToWorld(nodePoints[currentNode]);
	sf::Vector2f direction = worldPosition - node.GetSprite()->getPosition();

	if (Magnitude(direction) <= movementSpeed * Game::deltaTime)
	{
		return true;
	}

	return false;
}

void Enemy::MoveToCurrentNode()
{
	sf::Vector2f floatVersion;

	auto currentNodeWorldPosition = Game::GridToWorld(nodePoints[currentNode]);

	sf::Vector2f direction = currentNodeWorldPosition - node.GetPosition();
	direction = Normalise(direction);
	direction = Scale(direction, movementSpeed * Game::deltaTime);

	node.SetPosition(node.GetPosition() + direction);
}
