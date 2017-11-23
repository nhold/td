#ifndef GAMESTATE_HPP

#include <level.hpp>
#include <state.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <random>

class Level;

class GameState : public State
{
public:
	GameState(AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow);
	~GameState();

	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

	void SetLevel(std::string levelFileName);

private:
	Level currentLevel;
	sf::CircleShape towerRadius;
	sf::RenderWindow& renderWindow;

	Spawner<Enemy> enemySpawner;
	Spawner<Tower> towerSpawner;
	Spawner<Projectile> projectileSpawner;

	AssetDatabase& assetDatabase;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	// TODO: Extract to player.
	int currentGold;
	sf::Text goldText;
	sf::Sprite cursor;

	std::vector<Enemy*> deadEnemyVector;
	std::vector<Projectile*> deadProjectileVector;

	int currentWave;
	int currentData;
	float time = 0.1f;
	void UpdateWave();
	void UpdateEnemies();
	void UpdateTowers();
	void PostUpdate();

	void CreateTypes();
};

#endif // !GAMESTATE_HPP