#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>


class Node
{
public:
	Node();
	Node(sf::Sprite* aSprite, std::string aName);
	Node(const Node& otherEnemy);
	~Node();

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f newPosition);
	void SetSprite(sf::Sprite* aSprite);
	void SetFont(const sf::Font& font);
	void SetName(std::string aName);

	sf::Sprite* GetSprite();
	sf::Text& GetText();
	std::string GetName();
	sf::Vector2f GetPosition();
	void SetOrigin(float x, float y);

protected:
	sf::Text text;
	std::string name;
	sf::Sprite* sprite;


};

#endif
