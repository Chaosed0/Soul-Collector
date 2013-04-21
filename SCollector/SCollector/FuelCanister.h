/**
 * \brief A canister of fuel that extends the player's lighter time.
 * \author Edward Lu
 * \date April 21 2013
 */

#ifndef __FUELCANISTER_H
#define __FUELCANISTER_H

#include "Activatable.h"
class Level;

class FuelCanister : public Activatable 
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the canister within the level.
	 */
	FuelCanister(const sf::Vector2f& pos, int recovery);

	/**
	 * Update function.
	 * \param level The level in which the key is located.
	 * \param timePassed time past since the last time the function was called.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	/** The amount of fuel that the canister recovers. */
	int recovery;
};

#endif