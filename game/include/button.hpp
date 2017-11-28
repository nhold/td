#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <functional>
#include <node.hpp>

namespace sf 
{
	class Sprite;
}

class Button
{
public:
	Button();
	Button(sf::Sprite* sprite, std::string name, std::function<void()> onClicked);

	void Listen(std::function<void()> onClicked);
	void Invoke();

	Node node;

	void Update(sf::Vector2f mousePosition);
private:
	std::function<void()> onClicked;

};

#endif
