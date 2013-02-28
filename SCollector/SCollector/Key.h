/**
 * \brief In-game representation of a key.
 * \author Ed Lu
 * \date February 25 2013
 */

#ifndef __KEY_H
#define __KEY_H

#include "Activatable.h"
#include <list>
class Level;


class Key : public Activatable 
{
public:
	Key(sf::Vector2f pos);
	virtual void Update(const Level& level);
	void pickup(Player player);

private:
};
std::list<Key> myKeys;
#endif