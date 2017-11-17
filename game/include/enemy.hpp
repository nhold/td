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
	Enemy(int aHealth, int aMovementSpeed, int aDamage, int worth, sf::Sprite* aSprite, Path* aPath, std::string aName);
	Enemy(const Enemy& otherEnemy);
	~Enemy();

	int currentHealth;
	int maximumHealth;
	int movementSpeed;
	int damage;
	int worth;


	void SetPath(Path* path);


	void Update();
	Node node;
private:
	bool AtCurrentNode();
	void MoveToCurrentNode();
	std::vector<sf::Vector2i> nodePoints;
	int currentNode;

	
	
};

#endif
