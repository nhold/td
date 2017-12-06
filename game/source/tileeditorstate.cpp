#include <tileeditorstate.hpp>

#include <assethelper.hpp>
#include <game.hpp>
#include <SFML/Graphics/Sprite.hpp>

TileEditorState::TileEditorState(StateMachine & stateMachine, AssetDatabase & assetDatabase, sf::RenderWindow & renderWindow) : 
	assetDatabase(assetDatabase),
	renderWindow(renderWindow),
	stateMachine(stateMachine)
{

}

void TileEditorState::Initialise()
{
	tileMap.tileTypes[0] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/grass.png").resource);
	tileMap.tileTypes[1] = new sf::Sprite(assetDatabase.textureHandler.GetResource("assets/dirt.png").resource);
	tileMap.tileTypes[2] = CreateTempSprite(sf::Color::Blue);

	currentSelectedTile = 0;
	currentTile = tileMap.tileTypes[currentSelectedTile];
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
			tileMap.tiles[arrayPoint.x][arrayPoint.y] = currentSelectedTile;
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
		if (currentEvent.key.code == sf::Keyboard::Num1)
		{
			currentSelectedTile = 0;
			currentTile = tileMap.tileTypes[currentSelectedTile];
		}

		if (currentEvent.key.code == sf::Keyboard::Num2)
		{
			currentSelectedTile = 1;
			currentTile = tileMap.tileTypes[currentSelectedTile];
		}

		if (currentEvent.key.code == sf::Keyboard::Num3)
		{
			currentSelectedTile = 2;
			currentTile = tileMap.tileTypes[currentSelectedTile];
		}
	}

	if (currentEvent.type == sf::Event::KeyReleased)
	{
		if (currentEvent.key.code == sf::Keyboard::S)
		{
			tileMap.SaveToFile("assets/temporary.txt");
		}
	}
}
