#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include <vector>
#include <mutex>
#include <nodesystem.hpp>

template<typename T>
class Spawner : public NodeSystem<T>
{
public:
	std::vector<T> types;

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
};

#endif // !SPAWNER_HPP
