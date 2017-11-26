#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include <vector>

// TODO: More like a pool?
template<typename T>
class Spawner
{
public:
	std::vector<T> types;
	std::vector<T*> instances;

	Spawner::Spawner()
	{
	}

	Spawner::~Spawner()
	{
		while (instances.size() > 0)
		{
			DespawnBack();
		}
	}

	void Spawner::AddType(T type)
	{
		types.push_back(type);
	}

	T* Spawner::Spawn(int typeID)
	{
		if (typeID < types.size())
		{
			auto eT = new T(types[typeID]);
			eT->node.isAlive = true;
			instances.push_back(eT);

			return eT;
		}

		return nullptr;
	}

	void Spawner::DespawnBack()
	{
		if (instances.size() > 0)
		{
			auto instance = instances.back();
			instances.pop_back();
			delete instance;
		}
	}

	void DespawnAll()
	{
		while (instances.size() > 0)
		{
			auto instance = instances.back();
			instances.pop_back();
			delete instance;
		}
	}

	std::vector<T*> InArea(sf::Vector2f center, float radius)
	{
		std::vector<T*> withinArea;

		for (auto it = instances.begin(); it != instances.end(); ++it)
		{
			auto type = (*it);

			auto vec = type->node.GetPosition() - center;

			float distance = Magnitude(vec);
			if (distance < radius)
			{
				withinArea.push_back(type);
			}
		}

		return withinArea;
	}
};

#endif // !SPAWNER_HPP
