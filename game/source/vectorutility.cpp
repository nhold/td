#include <vectorutility.hpp>
#include <cmath>

sf::Vector2f Normalise(sf::Vector2f initial)
{
	if (initial.x == 0 && initial.y == 0)
		return initial;

	float val = Magnitude(initial);

	sf::Vector2f rValue;
	rValue.x = initial.x / val;
	rValue.y = initial.y / val;
	return rValue;
}

float Magnitude(sf::Vector2f initial) 
{
	float val = std::sqrt(std::pow(initial.x, 2) + std::pow(initial.y, 2));
	return val;
}


sf::Vector2f Scale(sf::Vector2f scaleValue, float scaleAmount)
{
	scaleValue.x *= scaleAmount;
	scaleValue.y *= scaleAmount;

	return scaleValue;
}
