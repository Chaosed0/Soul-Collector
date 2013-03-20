/**
 * \brief In-game representation of a torch.
 * \author Ed Lu
 * \date March 14 2013
 */

#ifndef __TORCH_H
#define __TORCH_H

#include "Activatable.h"
#include "LightSource.h"
class Player;
class Level;

class Torch : public Activatable 
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the torch within the level.
	 */
	Torch(const sf::Vector2f& pos);

	/**
	 * Update function.
	 * \param level The level in which the torch is located.
	 */
	virtual void Update(const Level& level, const sf::Time& timePassed);

	/**
	 * Adds the light source of the torch to the level.
	 * \param level The level.
	 */
	void AddLight(Level& level);
private:
	/** The light source this torch produces. */
	LightSource light;
};

#endif
