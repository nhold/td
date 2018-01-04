#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <string>
#include <node.hpp>

class Path;

class Enemy
{
public:
	Enemy();
	Enemy(int health, int movementSpeed, int damage, int worth, sf::Sprite* sprite, Path* path, std::string name);
	Enemy(const Enemy& otherEnemy);
	~Enemy();

	int currentHealth;
	int maximumHealth;
	int movementSpeed;
	int damage;
	int worth;

	Node node;

	void SetPath(Path* path);

	void Update(float deltaTime);

	void RenderHealthbars(sf::RenderWindow& window);
private:
	bool AtCurrentNode(float deltaTime);
	void MoveToCurrentNode(float deltaTime);
	sf::Sprite* healthBarSprite;
	std::vector<sf::Vector2i> nodePoints;
	int currentNode;

	
	
};

#endif
