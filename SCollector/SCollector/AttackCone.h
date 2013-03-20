/**
 * \brief Represents the bounding box of either the player's or a monster's attack.
 * \date March 19 2013
 * \author Edward Lu
 */

#ifndef __AttackCone_H
#define __AttackCone_H

#include <SFML/Graphics.hpp>

#include "Utility.h"

class Level;
class Movable;

class AttackCone
{
public:
	/**
	 * Constructor.
	 * \param time Time until this attack box expires.
	 * \param box The size of this attack box.
	 */
	AttackCone(const sf::Time& time, const sf::Vector2f& origin,
		float length, float angleBegin, float angleEnd);

	/**
	 * Update function, to be called once every loop.
	 * \param level The level this attack box is located in.
	 */
	void Update(const Level& level, const sf::Time& timePassed);

	/**
	 * Checks whether or not the attack box is still valid.
	 * \return True if the attack box is expired, false otherwise.
	 */
	bool IsExpired() const;

	/**
	 * Checks whether a point is within this attack cone.
	 * \return True if the point is within the cone, false otherwise.
	 */
	bool Contains(const sf::Vector2f point) const;

	/**
	 * Adds a Movable to the list of Movables already hit by the attack.
	 * \param movable The Movable hit by the attack.
	 */
	void MovableHit(const Movable& movable);

	/**
	 * Checks if a Movable has been hit by this attack.
	 * \param movable The Movable to check.
	 */
	bool IsMovableHit(const Movable& movable) const;

	/**
	 * Gets a triangle approximating the cone.
	 *
	 * For debug purposes only, so we just stick the definition in the header.
	 * \return An sf::Triangleshape representing the cone.
	 */
	sf::ConvexShape GetTriangle() const
	{
		sf::ConvexShape triangle;
		sf::Vector2f point1 = sf::Vector2f(length*cos(angleBegin), length*sin(angleBegin));
		sf::Vector2f point2 = sf::Vector2f(length*cos(angleEnd), length*sin(angleEnd));
		triangle.setPointCount(3);
		triangle.setPoint(0, sf::Vector2f(0.0f, 0.0f));
		triangle.setPoint(1, point1);
		triangle.setPoint(2, point2);
		triangle.setFillColor(sf::Color(0, 0, 0, 100));
		triangle.setOutlineColor(sf::Color(255, 255, 255, 200));
		triangle.setOutlineThickness(2);
		//printf("Draw cone at (%g, %g), (%g, %g)\n", point1.x, point1.y, point2.x, point2.y);
		return triangle;
	}
private:
	/** Point of the cone */
	sf::Vector2f origin;
	/** Length of the cone */
	float length;
	/** Start angle, in radians */
	float angleBegin;
	/** End angle, in radians */
	float angleEnd;

	/** How much time the attack should live for */
	sf::Time liveTime;
	/** How much time has passed since the box was created */
	sf::Time livedTime;
	/** Flag set to true when the attack box is no longer valid */
	bool isExpired;

	/** Vector of entities already hit by the attack (so they can't be hit multiple times) */
	std::vector<const Movable*> movablesHit;
};

#endif