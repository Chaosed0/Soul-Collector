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
	virtual void Attack(Movable& movable);

	/**
	 * Removes health from this movable.
	 *
	 * Prefer calling this over simply removing from health so that
	 *  the other movable can play a sound or something.
	 */
	virtual void RemoveHealth(int amount);

	/**
	 * Gets whether or not the Movable is alive or not.
	 * \return True if the movable is alive, false otherwise.
	 */
	virtual bool IsAlive();
protected:
	/** Movement direction of the entity*/
	float moveAngle;
	/** Attack power of the entity */
	int attackPower;
	/** Health of the entity*/
	int health;

	/** Maximum health of the entity */
	static const int maxHealth;
	/** Default attack power of the entity */
	static const int defaultAttackPower;
private:
};

#endif