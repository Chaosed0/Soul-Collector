/**
 * \brief A potion that adds to the player's health.
 * \author Edward Lu
 * \date April 21 2013
 */

#ifndef __HEALTHPOT_H
#define __HEALTHPOT_H

#include "Activatable.h"
class Level;

class HealthPot : public Activatable 
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the key within the level.
	 */
	HealthPot(const sf::Vector2f& pos, int recovery);

	/**
	 * Update function.
	 * \param level The level in which the key is located.
	 * \param timePassed time past since the last time the function was called.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	/** The amount of health that the potion recovers. */
	int recovery;
};

#endif