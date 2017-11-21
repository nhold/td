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
};

#endif // !SPAWNER_HPP
