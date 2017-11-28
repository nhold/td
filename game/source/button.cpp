#include <button.hpp>

Button::Button() : node()
{
}

Button::Button(sf::Sprite * sprite, std::string name, std::function<void()> onClicked) : node(sprite, name)
{
	this->onClicked = onClicked;
}

void Button::Listen(std::function<void()> onClicked)
{
	this->onClicked = onClicked;
}

void Button::Invoke()
{
	onClicked();
}

void Button::Update(sf::Vector2f mousePosition)
{
	if (node.GetSprite()->getGlobalBounds().contains(mousePosition))
		Invoke();
}
