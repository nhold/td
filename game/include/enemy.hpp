#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>

class Path;

// TODO: Extract path follow logic?
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

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f newPosition);
	void SetPath(Path* path);
	void SetSprite(sf::Sprite* aSprite);
	void SetFont(const sf::Font& font);
	void SetName(std::string aName);

	sf::Sprite* GetSprite();
	sf::Text& GetText();
	std::string GetName();


	void Update();

private:
	sf::Text text;
	bool AtCurrentNode();
	std::string name;
	void MoveToCurrentNode();
	std::vector<sf::Vector2i> nodePoints;
	int currentNode;
	sf::Sprite* sprite;
	
	
};

#endif
