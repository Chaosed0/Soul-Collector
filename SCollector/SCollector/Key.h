/**
 * \brief In-game representation of a key.
 * \author Ed Lu
 * \date February 25 2013
 */

#ifndef __KEY_H
#define __KEY_H

#include "Entity.h"
class Level;

class Key : public Entity
{
public:
	Key(sf::Vector2f pos);
	virtual void Update(const Level& level);
private:
};

#endif