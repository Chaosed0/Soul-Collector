/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __SLOWDEMON_H
#define __SLOWDEMON_H

#include "Movable.h"
#include "Utility.h"

#define SPOT_RADIUS 200

class SlowDemon : public Movable
{
public:
	SlowDemon(sf::Vector2f pos);

	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	/**
	 * Gets the attack cone this monster produces.
	 * \return The attack cone.
	 */
	AttackCone GetAttackCone();

	enum {
		IDLE,
		MOVING,
		ALERT,
		ATTACKING,
		DEAD
	} state;

	/** Timer to keep track of how much time passed since the last action */
	sf::Time timer;

	//sf::ConvexShape shape;

	/** How much time the monster needs to attack */
	static const sf::Time attackTime;
	/** Speed, in pixels per second */
	static const float defaultMoveSpeed;
	/** Radius of the monster's attack cone */
	static const float attackRadius;
	/** Sweep of the monster's attack cone */
	static const float attackSweep;
};

#endif