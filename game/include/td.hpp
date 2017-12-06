#ifndef TD_HPP
#define TD_HPP

#include <game.hpp>
#include <level.hpp>
#include <enemyspawner.hpp>
#include <projectile.hpp>

#include <statemachine.hpp>

class GameState;
class MenuState;
class TileEditorState;

class Td : public Game
{
public:
	Td();
	~Td();

protected:
	void Initialise() override;
	void Update() override;
	void Render() override;
	void ProcessInput(sf::Event currentEvent) override;

private:
	GameState* gameState;
	MenuState* menuState;
	TileEditorState* tileEditState;
	StateMachine stateMachine;

};

#endif
