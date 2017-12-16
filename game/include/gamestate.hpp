#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <level.hpp>
#include <state.hpp>
#include <statemachine.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <random>
#include <thread>
#include <mutex>

class MenuState;

class GameState : public State
{
public:
	GameState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow);
	~GameState();

	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	void MultithreadedUpdate();
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

	void SetLevel(std::string levelFileName);

	MenuState* menuState;
private:
	bool running;
	Level currentLevel;
	StateMachine& stateMachine;
	sf::CircleShape towerRadius;
	sf::RenderWindow& renderWindow;
	std::thread* updateThread;
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
	int currentSelectedTower;
	float time = 0.1f;
	void UpdateWave();
	void UpdateEnemies();
	void UpdateTowers();
	void PostUpdate();

	void CreateTypes();
	void DestroyTypes();
};

#endif // !GAMESTATE_HPP
