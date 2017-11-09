#ifndef VECTORUTILITY_HPP
#define VECTORUTILITY_HPP

#include <SFML/System/Vector2.hpp>


sf::Vector2f Normalise(sf::Vector2f initial);
float Magnitude(sf::Vector2f initial);
sf::Vector2f Scale(sf::Vector2f scaleValue, float scaleAmount);

#endif //VECTORUTILITY_HPP
