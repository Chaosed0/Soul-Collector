/**
 * \brief In-game representation of a key.
 * \author Ed Lu
 * \date February 25 2013
 */

#ifndef __KEY_H
#define __KEY_H

#include <list>

#include "Activatable.h"
class Player;
class Level;

class Key : public Activatable 
{
public:
	Key(sf::Vector2f pos);
	virtual void Update(const Level& level);
	void pickup(const Player& player);

private:
};

#endif
