#include <base.hpp>
#include <enemy.hpp>
#include <string>

Base::Base()
{
}

Base::Base(int health, sf::Sprite * sprite, std::string name)
{
	node.SetSprite(sprite);
	node.SetName(name);

	this->health = health;
}

void Base::Update(std::vector<Enemy*>& enemies)
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		Enemy* enemy = (*it);
		if (enemy->currentHealth > 0)
		{
			if (enemy->node.GetSprite()->getGlobalBounds().intersects(node.GetSprite()->getGlobalBounds()))
			{
				health -= enemy->damage;
				enemy->currentHealth = 0; // Kill it next frame.
				enemy->node.isAlive = false;
				enemy->worth = 0; // Remove it's worth so we don't get gold.
			}
		}
	}
	
}
