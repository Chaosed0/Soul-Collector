/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __DEMON_H
#define __DEMON_H

#include "Movable.h"
#include "Utility.h"

#define SPOT_RADIUS 200
#define CHARGE_RADIUS 100

class Demon : public Movable
{
public:
	Demon(sf::Vector2f pos);

	virtual void Update(const Level& level);
private:
	enum {
		IDLE,
		MOVING,
		ALERT,
		CHARGING
	} state;
	sf::Vector2f movement;

	/** Clock for general purpose */
	sf::Clock timer;

	/** How much time the monster is alerted before it decides to charge */
	static const sf::Time alertTime;
	/** How much time the monster is charging before it stops */
	static const sf::Time chargeTime;
};

#endif