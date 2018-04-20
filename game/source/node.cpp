#include <node.hpp>
#include <vectorutility.hpp>
#include <game.hpp>
#include <iostream>

Node::Node()
{
	sprite = nullptr;
	name = "null";
	text.setString(name);
	text.setFillColor(sf::Color::Magenta);
}

Node::Node(sf::Sprite * aSprite, std::string aName)
{
	SetSprite(aSprite);
	name = aName;
	text.setString(name);
	text.setFillColor(sf::Color::Magenta);
}

Node::Node(const Node & otherNode)
{
	if (otherNode.sprite != nullptr)
	{
		sprite = new sf::Sprite(*otherNode.sprite);
		SetPosition(otherNode.sprite->getPosition());
	}

	name = otherNode.name;
	text.setString(name);
	text.setFillColor(sf::Color::Magenta);

	if (otherNode.text.getFont() != nullptr)
	{
		text.setFont(*otherNode.text.getFont());
		text.setCharacterSize(12);
	}
}

Node::~Node()
{
	if (sprite != nullptr)
		delete sprite;
}

void Node::SetSprite(sf::Sprite * aSprite)
{
	sprite = new sf::Sprite(*aSprite);
	sprite->setOrigin(sprite->getTextureRect().width*0.5f, sprite->getTextureRect().height*0.5f);
}

void Node::SetPosition(float x, float y)
{
	sprite->setPosition(x, y);
	text.setPosition(x, y + 20);
}

void Node::SetPosition(sf::Vector2f newPosition)
{
	sprite->setPosition(newPosition);
	text.setPosition(newPosition + sf::Vector2f(0, 20));
}

void Node::SetDirection(sf::Vector2f direction)
{
	float rotAngle = ToAngle(direction);
	sprite->setRotation(rotAngle);
}

bool Node::IsPositionOver(const sf::Vector2f &position)
{
	return sprite->getGlobalBounds().contains(position);
}

sf::Vector2f Node::GetPosition()
{
	return sprite->getPosition();
}

void Node::SetOrigin(float x, float y)
{
	if (sprite != nullptr)
	{
		sprite->setOrigin(sprite->getTextureRect().width*x, sprite->getTextureRect().height*y);
	}
}

sf::Sprite * Node::GetSprite()
{
	return sprite;
}

sf::Text & Node::GetText()
{
	return text;
}

std::string Node::GetName()
{
	return name;
}

void Node::SetFont(const sf::Font & font)
{
	text.setFont(font);
	text.setCharacterSize(12);
}

void Node::SetName(std::string aName)
{
	name = aName;
}
