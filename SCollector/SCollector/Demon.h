/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __DEMON_H
#define __DEMON_H

#include "Movable.h"

class Demon : public Movable
{
public:
	Demon(sf::Vector2f pos);

	virtual void Update(const Level& level);
private:
	bool moving;
	bool moveHoriz;
	bool movePos;
};

#endif