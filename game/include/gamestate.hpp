#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <level.hpp>
#include <state.hpp>
#include <statemachine.hpp>
#include <button.hpp>
#include <enemyspawner.hpp>

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
	void SetupBuildTowerButtons();
	virtual void Shutdown() override;
	virtual void Update() override;
	void MultithreadedUpdate();
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

	void SetLevel(std::string levelFileName);

	MenuState* menuState;
private:

	void UpdateSelectedTower(int selectedTowerIndex);
	bool running;
	Level currentLevel;
	StateMachine& stateMachine;
	sf::CircleShape towerRadius;
	sf::RenderWindow& renderWindow;
	std::thread* updateThread;

	EnemySpawner enemySystem;
	Spawner<Tower> towerSpawner;
	Spawner<Projectile> projectileSpawner;
	NodeSystem<Button> buttonSpawner;

	AssetDatabase& assetDatabase;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	// TODO: Extract to player.
	int currentGold;
	sf::Text goldText;
	sf::Sprite cursor;

	std::vector<Projectile*> deadProjectileVector;

	int currentWave;
	int currentData;
	int currentSelectedTower;
	float time = 0.1f;
	float threadDeltaTime;
	sf::Clock clock;

	void UpdateWave(float deltaTime);
	void UpdateTowers(float deltaTime);
	void PostUpdate();

	void CreateTypes();
	void DestroyTypes();
};

#endif // !GAMESTATE_HPP
