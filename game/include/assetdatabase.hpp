#ifndef ASSETDATABASE_HPP
#define ASSETDATABASE_HPP

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

template<class T>
class Resource
{
public:
	Resource() : resource()
	{

	}

	~Resource()
	{

	}

	T resource;
	bool isValid;
};

template<class T>
class ResourceHandler
{
public:
	Resource<T>& GetResource(std::string filename) 
	{
		auto it = cache.find(filename);

		if (it != cache.end())
		{
			return (*it).second;
		}

		Resource<T> resource;
		resource.isValid = resource.resource.loadFromFile(filename);
		cache[filename] = resource;

		return cache[filename];
	}

private:
	std::map<std::string, Resource<T>> cache;
};

class AssetDatabase
{
public:
//	sf::Texture& GetTexture(std::string fileName);

	ResourceHandler<sf::Texture> textureHandler;
	ResourceHandler<sf::Font> fontHandler;
	ResourceHandler<sf::SoundBuffer> soundBuffer;
private:
//	std::map<std::string, sf::Texture> textureCache;
};

#endif // !ASSETDATABASE_HPP
