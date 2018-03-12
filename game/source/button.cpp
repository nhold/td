#include <button.hpp>

Button::Button() : node()
{
}

Button::Button(sf::Sprite * sprite, sf::Font* font, std::string name, std::function<void()> onClicked) : node(sprite, name)
{
	this->onClicked = onClicked;
	this->text = new sf::Text(name, *font);
	this->text->setCharacterSize(16);
	this->text->setOrigin(this->text->getLocalBounds().width / 2, this->text->getLocalBounds().height / 2);
}

void Button::Listen(std::function<void()> onClicked)
{
	this->onClicked = onClicked;
}

void Button::Invoke()
{
	onClicked();
}

bool Button::Update(sf::Vector2f mousePosition)
{
	if (IsPositionOver(mousePosition))
	{
		Invoke();
		return true;
	}

	return false;
}

bool Button::IsPositionOver(sf::Vector2f position)
{
	return node.GetSprite()->getGlobalBounds().contains(position);
}

void Button::SetPosition(float x, float y)
{
	node.SetPosition(x, y);
	text->setPosition(x, y);
}

void Button::Render(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*node.GetSprite());
	renderWindow.draw(*text);
}
