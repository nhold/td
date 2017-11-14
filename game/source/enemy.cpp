#include <enemy.hpp>
#include <path.hpp>
#include <vectorutility.hpp>
#include <game.hpp>
#include <iostream>

Enemy::Enemy()
{
	currentHealth = 1;
	maximumHealth = 1;
	movementSpeed = 1;
	damage = 1;
	sprite = nullptr;
	worth = 1;

	name = "null";
	text.setString(name);
	text.setColor(sf::Color::Magenta);
	currentNode = 0;
	std::cout << text.getString().toAnsiString() << " Default Constructor." << std::endl;
}

Enemy::Enemy(int aHealth, int aMovementSpeed, int aDamage, int worth, sf::Sprite * aSprite, Path * aPath, std::string aName)
{
	currentHealth = aHealth;
	maximumHealth = currentHealth;
	movementSpeed = aMovementSpeed;

	damage = aDamage;
	SetSprite(aSprite);
	SetPath(aPath);
	name = aName;
	this->worth = worth;
	text.setString(name);
	text.setColor(sf::Color::Magenta);
	currentNode = 0;
	std::cout << text.getString().toAnsiString() << " Data Constructor. " << std::endl;
}

Enemy::Enemy(const Enemy & otherEnemy)
{
	currentHealth = otherEnemy.currentHealth;
	maximumHealth = otherEnemy.maximumHealth;
	movementSpeed = otherEnemy.movementSpeed;
	damage = otherEnemy.damage;
	worth = otherEnemy.worth;
	if (otherEnemy.sprite != nullptr)
	{
		sprite = new sf::Sprite(*otherEnemy.sprite);
		SetPosition(otherEnemy.sprite->getPosition());
	}

	nodePoints = otherEnemy.nodePoints;
	currentNode = otherEnemy.currentNode;
	name = otherEnemy.name;
	text.setString(name);
	text.setColor(sf::Color::Magenta);
	

	if (otherEnemy.text.getFont() != nullptr)
	{
		text.setFont(*otherEnemy.text.getFont());
		text.setCharacterSize(12);
	}

	std::cout << text.getString().toAnsiString() << " Copy Constructor" << std::endl;
}

Enemy::~Enemy()
{
	if(sprite != nullptr)
		delete sprite;
}

void Enemy::SetPath(Path* path)
{
	if (path == nullptr)
		return;

	nodePoints = path->nodePoints;
	currentNode = 0;
	std::cout << text.getString().toAnsiString() <<  " Set Path " << std::endl;
}

void Enemy::SetSprite(sf::Sprite * aSprite)
{
	sprite = new sf::Sprite(*aSprite);
	sprite->setOrigin(sprite->getTextureRect().width*0.5f, sprite->getTextureRect().height*0.5f);
}

void Enemy::SetPosition(float x, float y)
{
	sprite->setPosition(x, y);
	text.setPosition(x, y + 20);
}

void Enemy::SetPosition(sf::Vector2f newPosition)
{
	sprite->setPosition(newPosition);
	text.setPosition(newPosition + sf::Vector2f(0, 20));
}

sf::Sprite * Enemy::GetSprite()
{
	return sprite;
}

sf::Text & Enemy::GetText()
{
	return text;
}

std::string Enemy::GetName()
{
	return name;
}

void Enemy::SetFont(const sf::Font & font)
{
	text.setFont(font);
	text.setCharacterSize(12);
}

void Enemy::SetName(std::string aName)
{
	name = aName;
}
	
void Enemy::Update()
{
	if (nodePoints.size() == 0)
		return;

	if (currentNode < nodePoints.size())
	{
		text.setString(name + " : Health: " + std::to_string(currentHealth) +
			" - Nodes: " + std::to_string(currentNode) + " \ " + std::to_string(nodePoints.size()));
		MoveToCurrentNode();

		if (AtCurrentNode())
			currentNode++;
	}
	else 
	{
		currentNode = 0;
		SetPosition(Game::GridToWorld(nodePoints[currentNode]));
		std::cout << text.getString().toAnsiString() << std::endl;
	}
}

bool Enemy::AtCurrentNode()
{
	sf::Vector2f worldPosition;
	worldPosition = Game::GridToWorld(nodePoints[currentNode]);
	sf::Vector2f direction = worldPosition - sprite->getPosition();

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

	sf::Vector2f direction = currentNodeWorldPosition - sprite->getPosition();
	direction = Normalise(direction);
	direction = Scale(direction, movementSpeed * Game::deltaTime);

	SetPosition(sprite->getPosition() + direction);
}
