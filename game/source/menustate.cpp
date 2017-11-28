#include <menustate.hpp>
#include <gamestate.hpp>
#include <iostream>

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

	auto button = buttonSpawner.Spawn(0);
	button->Listen(std::bind(&MenuState::ButtonClicked, this));
	levelSet = false;
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
			if (levelSet)
				break;
		}
	}

	if(levelSet)
		stateMachine.SetState(gameState);
}

void MenuState::Render()
{
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

void MenuState::ButtonClicked()
{
	gameState->SetLevel("assets/level1_metadata.txt");
	levelSet = true;
}
