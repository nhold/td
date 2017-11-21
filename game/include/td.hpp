#ifndef TD_HPP
#define TD_HPP

#include <game.hpp>
#include <level.hpp>
#include <enemyspawner.hpp>
#include <projectile.hpp>

class Td : public Game
{
public:
	Td();
	~Td();

protected:
	void Initialise() override;
	void Update() override;
	void Render() override;
	void ProcessInput(sf::Event currentEvent) override;
	

private:
	

};

#endif
