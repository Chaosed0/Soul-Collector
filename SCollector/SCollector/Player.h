/**
 * \brief Represents the player's avatar.
 * \author Edward Lu
 * \date January 28 2013
 *
 * This is a subclass of sf::Drawable so that it can be drawn by itself.
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <SFML/Graphics.hpp>

#include "Movable.h"
#include "LightSource.h"
class Level;

class Player : public Movable 
{
//Public attributes; visible to everyone
public:
	/**
	 * Init constructor.
	 * Initializes the player with no graphical component whatsoever - Make sure to
	 *  call LoadTexture() before drawing the player!
	 */
	Player(const sf::Vector2f& pos);

	/**
	 * Starts or stops the player moving left.
	 * \param start If true, the player starts moving left; otherwise, they stop
	 *  moving left.
	 */
	void MoveLeft(bool start);
	/**
	 * Starts or stops the player moving right.
	 * \param start If true, the player starts moving right; otherwise, they stop
	 *  moving right.
	 */
	void MoveRight(bool start);
	/**
	 * Starts or stops the player moving up.
	 * \param start If true, the player starts moving up; otherwise, they stop
	 *  moving up.
	 */
	void MoveUp(bool start);
	/**
	 * Starts or stops the player moving down.
	 * \param start If true, the player starts moving down; otherwise, they stop
	 *  moving down.
	 */
	void MoveDown(bool start);

	/**
	 * Turns the player's sprinting on or off.
	 * \param start True if the player should start sprinting, false otherwise.
	 */
	void Sprint(bool start);

	/**
	 * Adds the player's light to the level so it can be drawn.
	 * \param level The level.
	 */
	void AddLight(Level& level);

	/**
	 * Updates the player's position.
	 * This function should be called once per frame.
	 */
	virtual void Update(const Level& level, const sf::Time& timePassed);

	/**
	 * Toggles the player's lighter on/off state
	 */
	void ToggleLighter();

//Private attributes; visible only within this class
private:
	/** Flags set to true when the player should move in the corresponding direction */
	bool moveLeft, moveRight, moveUp, moveDown;

	/** Player's light source */
	LightSource lighter;
	/** Player's light source when the lighter is off */
	LightSource ambientLight;

	/** True when the player is sprinting, false when he is walking at regSpeed */
	bool isSprinting;

	/** Amount of fatigue the player has - directly proportional to the time he can run. */
	int fatigue;

	/** Regular walking speed, in pixels per second. */
	static const float regSpeed;
	/** Running speed, in pixels per second. */
	static const float sprintSpeed;
	/** Max amount of fatigue. */
	static const int maxFatigue;
};

#endif