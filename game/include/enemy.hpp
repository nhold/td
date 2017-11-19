#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
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

	void Update();
private:
	bool AtCurrentNode();
	void MoveToCurrentNode();

	std::vector<sf::Vector2i> nodePoints;
	int currentNode;

	
	
};

#endif
