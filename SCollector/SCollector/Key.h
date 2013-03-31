/**
 * \brief In-game representation of a key.
 * \author Ed Lu
 * \date February 25 2013
 */

#ifndef __KEY_H
#define __KEY_H

#include "Activatable.h"
class Level;

class Key : public Activatable 
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the key within the level.
	 */
	Key(const sf::Vector2f& pos);

	/**
	 * Update function.
	 * \param level The level in which the key is located.
	 * \param timePassed time past since the last time the function was called.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);

	/**
	 * Gets the name of the door the key opens.
	 * \return The name of the door that the key should open.
	 */
	std::string GetDoorName();

	void setKeyID(int keyID);
	int getKeyID();
private:
	/** The name of the door this key opens. */
	std::string doorName;
	int KeyID;
};

#endif
