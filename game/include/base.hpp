#ifndef BASE_HPP
#define BASE_HPP

#include <node.hpp>

class Enemy;

class Base
{
public:
	Base();
	Base(int health, sf::Sprite* aSprite, std::string aName);

	void Update(std::vector<Enemy*>& enemies);

	int health;
	Node node;
};

#endif // !BASE_HPP
