#ifndef WAVE_HPP
#define WAVE_HPP

#include <enemyspawndata.hpp>
#include <vector>
#include <string>

class Wave
{
public:
	std::vector<EnemySpawnData> enemySpawnData;
	void LoadFromFile(std::string fileName);
};

#endif // !WAVE_HPP
