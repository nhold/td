#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Window/Event.hpp>

class State 
{
public:
	virtual void Initialise() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ProcessInput(sf::Event currentEvent) = 0;
};

#endif // !STATE_HPP
