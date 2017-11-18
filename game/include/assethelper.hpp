#ifndef ASSETHELPER_HPP
#define ASSETHELPER_HPP

#include <SFML/Graphics/Color.hpp>

namespace sf
{
	class Sprite;
}

sf::Sprite* CreateTempSprite(const sf::Color &color, int length = 32, int height = 32);

#endif // !ASSETHELPER_HPP
