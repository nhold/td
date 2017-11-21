#ifndef MENUSTATE_HPP

#include <state.hpp>

class MenuState : public State
{
	// Inherited via State
	virtual void Initialise() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void ProcessInput(sf::Event currentEvent) override;
};

#endif // !MENUSTATE_HPP

