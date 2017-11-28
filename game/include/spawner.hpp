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

	Spawner()
	{
	}

	~Spawner()
	{
		while (instances.size() > 0)
		{
			DespawnBack();
		}
	}

	void AddType(T type)
	{
		types.push_back(type);
	}

	T* Spawn(int typeID)
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

	void DespawnBack()
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

	T* ClosestInArea(sf::Vector2f center, float radius, std::vector<T*> exclusionVec)
	{
		T* closest = nullptr;
		float maxDistance = 99999;

		for (auto it = instances.begin(); it != instances.end(); ++it)
		{
			auto type = (*it);

			if (std::find(exclusionVec.begin(), exclusionVec.end(), type) != exclusionVec.end())
				continue;

			auto vec = type->node.GetPosition() - center;

			float distance = Magnitude(vec);
			if (distance < maxDistance)
			{
				closest = type;
				maxDistance = distance;
			}
		}

		return closest;
	}
};

#endif // !SPAWNER_HPP
