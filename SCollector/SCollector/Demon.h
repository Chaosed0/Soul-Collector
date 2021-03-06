/**
 * \brief An enemy that charges the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __DEMON_H
#define __DEMON_H

#include "Movable.h"
#include "Utility.h"

#define SPOT_RADIUS 200

class Demon : public Movable
{
public:
	Demon(sf::Vector2f pos);

	virtual void Update(Level& level, const sf::Time& timePassed);

	/**
	 * Simply checks if the demon is currently charging (otherwise it can't
	 *  deal damage) and calls Movable::Attack if so
	 * \param movable The movable the demon is attacking.
	 */
	void Attack(Movable& movable);
private:
	enum {
		IDLE,
		MOVING,
		ALERT,
		CHARGING,
		RECOVERING,
		DEAD
	} state;

	/** Timer to keep track of how much time passed since the last action */
	sf::Time timer;

	//sf::ConvexShape shape;

	/** How much time the monster is alerted before it decides to charge */
	static const sf::Time alertTime;
	/** How much time the monster is charging before it stops */
	static const sf::Time chargeTime;
	/** How much time the monster is stunned after it charges */
	static const sf::Time stunTime;
	/** Wandering speed, in pixels per second */
	static const float wanderSpeed;
	/** Charging speed, in pixels per second */
	static const float chargeSpeed;
};

#endif