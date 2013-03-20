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

	/**
	 * Prompts this Movable to attack another.
	 *
	 * Simply subtracts this->attackPower from movable.health .
	 * \param movable The Movable that is getting attacked.
	 */
	void Attack(Movable& movable);
protected:
	/** Movement direction of the entity*/
	float moveAngle;
	/** Attack power of the entity */
	int attackPower;
	/** Health of the entity*/
	int health;
private:
};

#endif