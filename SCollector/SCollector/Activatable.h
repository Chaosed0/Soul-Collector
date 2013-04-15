/**
 * \brief Any in-game Entity that can be activated.
 * \author Edward Lu
 * \date February 28 2013
 */

#ifndef __ACTIVATABLE_H
#define __ACTIVATABLE_H

#include "Entity.h"

class Activatable : public Entity
{
public:
	Activatable(std::string imgLoc, sf::IntRect collisionBox,
		sf::IntRect animBox, sf::IntRect activateBox);
	
	void Activate();
	void Finish();
	bool IsFinished();
	bool IsActive();
protected:
	bool isActive;
	bool isFinished;
private:
	sf::IntRect activateBox;
};

#endif