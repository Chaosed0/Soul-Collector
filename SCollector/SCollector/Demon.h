/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __DEMON_H
#define __DEMON_H

#include "Movable.h"
#include "Utility.h"

#define SPOT_RADIUS 50

class Demon : public Movable
{
public:
	Demon(sf::Vector2f pos);

	virtual void Update(const Level& level);
private:
	bool moving;
	bool spotted;
	sf::Vector2f movement;
};

#endif