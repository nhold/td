#ifndef TOWER_HPP
#define TOWER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <node.hpp>
#include <vector>
#include <spawner.hpp>
#include <projectile.hpp>

class Enemy;

class Tower
{
public:
	Tower(int projectileID, int allowedTargetCount, float buildTime, float radius, float fireRate, int cost, sf::Sprite* sprite, std::string name, Spawner<Projectile>& projectileSpawner);
	Tower(const Tower& otherEnemy);
	~Tower();

	int projectileID;
	int numberOfTargets;
	float buildTime;
	bool isBuilding;
	float radius;
	float fireRate;
	int cost;

	void Update(std::vector<Enemy*>& allEnemies);
	sf::VertexArray GetDebugLines();
	Node node;
	Spawner<Projectile>& projectileSpawner;
private:
	void RemoveDeadTargets(std::vector<Enemy*>& allEnemies);
	void FindTarget(std::vector<Enemy*>& allEnemies);
	void Shoot();
	std::vector<Enemy*> targets;

	float currentRate;
};

#endif // !TOWER_HPP
