#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <state.hpp>
#include <statemachine.hpp>
#include <functional>
#include <button.hpp>
#include <spawner.hpp>
#include <assetdatabase.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameState;
class TileEditorState;

namespace sf {
	class Sprite;
}

class MenuState : public State
{
public:
	MenuState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow);
	virtual void Initialise() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;

	GameState* gameState;
	TileEditorState* tileEditState;
private:
	bool levelSet;
	bool levelEditSet;
	void PlayButtonClicked1();
	void PlayButtonClicked2();
	void PlayButtonClicked3();
	void LevelEditorButtonClicked();
	AssetDatabase& assetDatabase;
	StateMachine& stateMachine;
	sf::RenderWindow& renderWindow;
	sf::Sprite* background;
	Spawner<Button> buttonSpawner;
};

#endif // !MENUSTATE_HPP

