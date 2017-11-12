#ifndef TOWER_HPP
#define TOWER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

class Enemy;

class Tower
{
public:
	Tower();
	Tower(int aDamage, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* aSprite, std::string aName);
	Tower(const Tower& otherEnemy);
	~Tower();

	int damage;
	int numberOfTargets;
	float buildTime;
	bool isBuilding;
	float radius;
	float fireRate;
	int cost;

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f newPosition);
	
	void SetSprite(sf::Sprite* aSprite);
	void SetFont(const sf::Font& font);
	void SetName(std::string aName);

	sf::Sprite* GetSprite();
	sf::Text& GetText();
	std::string GetName();


	void Update(std::vector<Enemy*>& allEnemies);

private:
	void RemoveDeadTargets(std::vector<Enemy*>& allEnemies);
	void FindTarget(std::vector<Enemy*>& allEnemies);
	void Shoot();
	sf::Text text;
	sf::Sprite* sprite;
	std::vector<Enemy*> targets;
	std::string name;
	float currentRate;


};

#endif // !TOWER_HPP
