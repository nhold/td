#include <menustate.hpp>
#include <gamestate.hpp>
#include <iostream>
#include <tileeditorstate.hpp>

MenuState::MenuState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow) : stateMachine(stateMachine),
																												assetDatabase(assetDatabase),
																												renderWindow(renderWindow)
{
}

void MenuState::Initialise()
{
	sf::Sprite* buttonSprite = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/button.png"));
	buttonSpawner.AddType(Button(buttonSprite, "Button", [] {
		std::cout << "Button clicked" << std::endl;
	}));

	auto playButton = buttonSpawner.Spawn(0);
	playButton->Listen(std::bind(&MenuState::PlayButtonClicked, this));
	playButton->node.SetPosition(320, 320);

	auto tileEditButton = buttonSpawner.Spawn(0);
	tileEditButton->Listen(std::bind(&MenuState::LevelEditorButtonClicked, this));
	tileEditButton->node.SetPosition(320, 400);

	background = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/menuBackground.png"));
	levelSet = false;
	levelEditSet = false;
}

void MenuState::Shutdown()
{
	buttonSpawner.DespawnAll();
}

void MenuState::Update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		for each (auto button in buttonSpawner.instances)
		{
			button->Update(static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow)));
			if (levelSet || levelEditSet)
				break;
		}
	}

	if(levelSet)
		stateMachine.SetState(gameState);

	if (levelEditSet)
		stateMachine.SetState(tileEditState);
}

void MenuState::Render()
{
	renderWindow.draw(*background);
	for each (auto button in buttonSpawner.instances)
	{
		renderWindow.draw(*button->node.GetSprite());
	}
}

void MenuState::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::KeyReleased)
	{
		if (currentEvent.key.code == sf::Keyboard::Num1)
		{
			// Tell the game state what level it will be playing.
			gameState->SetLevel("assets/level1_metadata.txt");
			stateMachine.SetState(gameState);
		}
	}
}

void MenuState::PlayButtonClicked()
{
	gameState->SetLevel("assets/level1_metadata.txt");
	levelSet = true;
}

void MenuState::LevelEditorButtonClicked()
{
	levelEditSet = true;
}
