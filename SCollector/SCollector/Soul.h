/**
 * \brief A piece of the player's soul.
 * \author Edward Lu
 * \date April 19 2013
 */

#include "Activatable.h"

#ifndef __SOUL_H 
#define __SOUL_H

#include "Activatable.h"
class Level;

class Soul : public Activatable 
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the key within the level.
	 */
	Soul(const sf::Vector2f& pos);

	/**
	 * Update function.
	 * \param level The level in which the key is located.
	 * \param timePassed time past since the last time the function was called.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
};

#endif
