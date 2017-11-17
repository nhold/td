#ifndef TOWER_HPP
#define TOWER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <node.hpp>
#include <vector>

class Enemy;

class Tower
{
public:
	Tower();
	Tower(int aDamage, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* sprite, std::string name);
	Tower(const Tower& otherEnemy);
	~Tower();

	int damage;
	int numberOfTargets;
	float buildTime;
	bool isBuilding;
	float radius;
	float fireRate;
	int cost;


	void Update(std::vector<Enemy*>& allEnemies);
	Node node;
private:
	void RemoveDeadTargets(std::vector<Enemy*>& allEnemies);
	void FindTarget(std::vector<Enemy*>& allEnemies);
	void Shoot();
	std::vector<Enemy*> targets;

	float currentRate;


};

#endif // !TOWER_HPP
