#ifndef TILEEDITORSTATE_HPP
#define TILEDEDITORSTATE_HPP

#include <state.hpp>
#include <statemachine.hpp>
#include <functional>
#include <button.hpp>
#include <spawner.hpp>
#include <assetdatabase.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tilemap.hpp>


namespace sf
{
	class Sprite;
}

class MenuState;

class TileEditorState : public State
{
public:
	TileEditorState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow);
	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

	MenuState* menuState;
private:
	TileMap tileMap;

	int currentSelectedTile;
	AssetDatabase& assetDatabase;
	StateMachine& stateMachine;
	sf::RenderWindow& renderWindow;
	sf::Sprite* currentTile;
	Spawner<Button> buttonSpawner;
};

#endif // !TILEEDITORSTATE_HPP

