#ifndef VECTORUTILITY_HPP
#define VECTORUTILITY_HPP

#include <SFML/System/Vector2.hpp>
#include <string>

sf::Vector2f Normalise(sf::Vector2f initial);
float Magnitude(sf::Vector2f initial);
sf::Vector2f Scale(sf::Vector2f scaleValue, float scaleAmount);

template<typename T>
std::string ToString(sf::Vector2<T> value)
{
	std::string rValue;
	rValue = "X: " + std::to_string(value.x) + " . Y: " + std::to_string(value.y);
	return rValue;
}


#endif //VECTORUTILITY_HPP
