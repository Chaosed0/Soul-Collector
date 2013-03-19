/**
 * \brief Represents the bounding box of either the player's or a monster's attack.
 * \date March 19 2013
 * \author Edward Lu
 */

#ifndef __ATTACKBOX_H
#define __ATTACKBOX_H

#include <SFML/Graphics.hpp>

class Level;

class AttackBox : public sf::IntRect
{
public:
	/**
	 * Constructor.
	 * \param time Time until this attack box expires.
	 * \param box The size of this attack box.
	 */
	AttackBox(const sf::Time& time, const sf::IntRect& box);

	/**
	 * Update function, to be called once every loop.
	 * \param level The level this attack box is located in.
	 */
	void Update(const Level& level);

	/**
	 * Checks whether or not the attack box is still valid.
	 * \return True if the attack box is expired, false otherwise.
	 */
	bool IsExprired();
private:
	/** How much time the attack should live for */
	sf::Time liveTime;
	/** Time since the attack box was created */
	sf::Clock timer;
	/** Flag set to true when the attack box is no longer valid */
	bool isExpired;
};

#endif