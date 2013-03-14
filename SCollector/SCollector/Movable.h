/**
 * \brief Describes an object that can move by itself.
 * \author Ed Lu
 * \date March 2 2013
 */

#ifndef __MOVABLE_H
#define __MOVABLE_H

#include "Entity.h"

class Movable : public Entity
{
public:
	Movable(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox);
protected:
	/** Movement speed of the entity, in pixels per frame */
	float moveSpeed;
	/** Attack power of the entity */
	int attackPower;
	/** Health of the entity*/
	int health;
private:
};

#endif