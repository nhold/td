#include <menustate.hpp>
#include <gamestate.hpp>
#include <iostream>
#include <tileeditorstate.hpp>
#include <SFML/Graphics/Font.hpp>

MenuState::MenuState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow) : stateMachine(stateMachine),
																												assetDatabase(assetDatabase),
																												renderWindow(renderWindow)
{
}

void MenuState::Initialise()
{
	sf::Sprite* buttonSprite = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/button.png"));
	sf::Font* buttonFont = new sf::Font(assetDatabase.fontHandler.GetResource("assets/Consolas.ttf"));

	buttonSpawner.AddType(Button(buttonSprite, buttonFont, "Level 1", [] {
		std::cout << "Button clicked" << std::endl;
	}));

	buttonSpawner.AddType(Button(buttonSprite, buttonFont, "Level 2", [] {
		std::cout << "Button clicked" << std::endl;
	}));

	buttonSpawner.AddType(Button(buttonSprite, buttonFont, "Level 3", [] {
		std::cout << "Button clicked" << std::endl;
	}));

	buttonSpawner.AddType(Button(buttonSprite, buttonFont, "Tilemap Editor", [] {
		std::cout << "Button clicked" << std::endl;
	}));

	auto level1Button = buttonSpawner.Spawn(0);
	level1Button->Listen(std::bind(&MenuState::PlayButtonClicked1, this));
	level1Button->SetPosition(320, 320);

	auto level2Button = buttonSpawner.Spawn(1);
	level2Button->Listen(std::bind(&MenuState::PlayButtonClicked2, this));
	level2Button->SetPosition(320, 400);

	auto level3Button = buttonSpawner.Spawn(2);
	level3Button->Listen(std::bind(&MenuState::PlayButtonClicked3, this));
	level3Button->SetPosition(320, 480);

	auto tileEditButton = buttonSpawner.Spawn(3);
	tileEditButton->Listen(std::bind(&MenuState::LevelEditorButtonClicked, this));
	tileEditButton->SetPosition(320, 560);

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

}

void MenuState::Render()
{
	renderWindow.draw(*background);
	for each (auto button in buttonSpawner.instances)
	{
		button->Render(renderWindow);
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

	if (currentEvent.type == sf::Event::MouseButtonReleased)
	{
		if (currentEvent.mouseButton.button == sf::Mouse::Button::Left)
		{
			for each (auto button in buttonSpawner.instances)
			{
				button->Update(static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow)));
				if (levelSet || levelEditSet)
					break;
			}

			if (levelSet)
				stateMachine.SetState(gameState);

			if (levelEditSet)
				stateMachine.SetState(tileEditState);
		}
	}
}

void MenuState::PlayButtonClicked1()
{
	gameState->SetLevel("assets/level1_metadata.txt");
	levelSet = true;
}

void MenuState::PlayButtonClicked2()
{
	gameState->SetLevel("assets/level2_metadata.txt");
	levelSet = true;
}

void MenuState::PlayButtonClicked3()
{
	gameState->SetLevel("assets/level3_metadata.txt");
	levelSet = true;
}


void MenuState::LevelEditorButtonClicked()
{
	levelEditSet = true;
}
