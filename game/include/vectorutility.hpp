#ifndef VECTORUTILITY_HPP
#define VECTORUTILITY_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <mathutility.hpp>
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

template<typename T>
float ToAngle(sf::Vector2<T> value)
{
	value = Normalise(value);
	return (std::atan2(value.y, value.x) * PI_F_180);
}

#endif //VECTORUTILITY_HPP
