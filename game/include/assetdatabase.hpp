#ifndef ASSETDATABASE_HPP
#define ASSETDATABASE_HPP

#include <map>
#include <SFML/Graphics/Texture.hpp>

class AssetDatabase
{
public:
	sf::Texture& GetTexture(std::string fileName);

private:
	std::map<std::string, sf::Texture> textureCache;
};

#endif // !ASSETDATABASE_HPP
