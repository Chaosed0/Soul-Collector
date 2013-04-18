/**
 * \brief Any in-game Entity that can be activated.
 * \author Edward Lu
 * \date February 28 2013
 */

#ifndef __ACTIVATABLE_H
#define __ACTIVATABLE_H

#include "Entity.h"

#define DRAW_ACTCOLBOXES 0

class Activatable : public Entity
{
public:
	Activatable(std::string imgLoc, sf::IntRect collisionBox,
		sf::IntRect animBox, sf::IntRect activateBox);
	
	void Activate();
	bool IsFinished();
	bool IsActive();
	/**
	 * If entity is within range of this activatable, this entity
	 *  activates; otherwise, nothing happens.
	 * \param entity The entity attempting to activate this activatable.
	 */
	bool TryActivate(const Entity& entity);
protected:
	bool isActive;
	bool isFinished;
	void Finish();
private:
	sf::IntRect activateBox;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif