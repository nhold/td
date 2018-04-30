#ifndef NODESYSTEM_HPP
#define NODESYSTEM_HPP

#include <vector>
#include <mutex>

template<typename T>
class NodeSystem
{
public:
	std::vector<T*> instances;
	std::mutex mutex;

	NodeSystem()
	{
	}

	~NodeSystem()
	{
		DespawnAll();
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

	bool IsPositionOver(const sf::Vector2f &position, T** outHit)
	{
		for (auto it = instances.begin(); it != instances.end(); ++it)
		{
			if ((*it)->node.IsPositionOver(position))
			{
				*outHit = (*it);
				return true;
			}
		}

		return false;
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

			if (!type->node.isAlive)
				continue;

			if (std::find(exclusionVec.begin(), exclusionVec.end(), type) != exclusionVec.end())
				continue;

			auto vec = type->node.GetPosition() - center;
			float distance = Magnitude(vec);

			if (distance > radius)
				continue;

			if (distance < maxDistance)
			{
				closest = type;
				maxDistance = distance;
			}
		}

		return closest;
	}
};

#endif // !NODESYSTEM_HPP
