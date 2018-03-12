#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <functional>
#include <node.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sf 
{
	class Sprite;
	class Text;
	class Font;
}

class Button
{
public:
	Button();
	Button(sf::Sprite* sprite, sf::Font* font, std::string name, std::function<void()> onClicked);

	void Listen(std::function<void()> onClicked);
	void Invoke();

	Node node;

	bool Update(sf::Vector2f mousePosition);
	bool IsPositionOver(sf::Vector2f position);
	void SetPosition(float x, float y);
	void Render(sf::RenderWindow& renderTarget);

private:
	std::function<void()> onClicked;
	sf::Text* text;

};

#endif
