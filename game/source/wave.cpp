#include <wave.hpp>
#include <iostream>
#include <fstream>

void Wave::LoadFromFile(std::string fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{
		int i = 0;
		EnemySpawnData data;
		while (getline(myfile, line))
		{
			auto index = line.find_first_of(":");
			auto type = line.substr(0, index);
			auto spawnTime = line.substr(index+1);
			data.spawnTime = stof(spawnTime);
			data.type = stoi(type);
			this->enemySpawnData.push_back(data);
		}

		myfile.close();
	}
}
