#include <tileeditorstate.hpp>

#include <assethelper.hpp>
#include <algorithm>
#include <menustate.hpp>
#include <game.hpp>
#include <SFML/Graphics/Sprite.hpp>

TileEditorState::TileEditorState(StateMachine& stateMachine, AssetDatabase& assetDatabase, sf::RenderWindow& renderWindow) : 
	assetDatabase(assetDatabase),
	renderWindow(renderWindow),
	stateMachine(stateMachine)
{
}

void TileEditorState::Initialise()
{
	tileMap.tileTypes[0] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/grass.png").resource);
	tileMap.tileTypes[1] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/dirt.png").resource);
	tileMap.tileTypes[2] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/water.png").resource);
	tileMap.tileTypes[3] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/cliff.png").resource);

	currentSelectedTile = 0;
	currentTile = new sf::Sprite(*tileMap.tileTypes[currentSelectedTile]);
	currentTile->setOrigin(16, 16);
	currentTile->setColor(sf::Color(255, 255, 255, 128));
}

void TileEditorState::Shutdown()
{
}

void TileEditorState::Update()
{
	if (currentTile != nullptr)
	{
		auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow));
		
		currentTile->setPosition(Game::WorldToGrid(mousePosition));

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			auto arrayPoint = Game::WorldToArray(mousePosition);
			tileMap.tiles[tileMap.index(arrayPoint.x, arrayPoint.y)] = currentSelectedTile;
		}
	}
}

void TileEditorState::Render()
{
	tileMap.Render(&renderWindow);
	renderWindow.draw(*currentTile);
}

void TileEditorState::ProcessInput(sf::Event currentEvent)
{
	if (currentEvent.type == sf::Event::KeyPressed)
	{
		// We map the keycodes for 1, 2, 3 for 0, 1, 2 and clamp to 0,2 to ensure it stays valid.
		int nextTile = ((int)currentEvent.key.code - sf::Keyboard::Num1);
		if (nextTile >= 0 && nextTile < 4) // Ensure it's an actual numerical keycode.
			currentSelectedTile = std::clamp(nextTile, 0, 3);

		printf("Current Selected Tile: %i\n/n", currentSelectedTile);
		currentTile->setTexture(*tileMap.tileTypes[currentSelectedTile]->getTexture());
	}

	if (currentEvent.type == sf::Event::KeyReleased)
	{
		if (currentEvent.key.code == sf::Keyboard::S)
		{
			tileMap.SaveToFile("assets/temporary.txt");
		}

		if (currentEvent.key.code == sf::Keyboard::R)
		{
			printf("Current Selected Tile: %i", currentSelectedTile);
			tileMap.ResetToTile(currentSelectedTile);
		}

		if (currentEvent.key.code == sf::Keyboard::Escape)
		{
			stateMachine.SetState(menuState);
		}
	}
}
