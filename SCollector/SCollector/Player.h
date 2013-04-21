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
#include <set>

#include "Movable.h"
#include "LightSource.h"
class Level;
class AttackCone;
class HUD;

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
	 * Gets the player's AttackCone.
	 * \return An AttackCone representing the area the player would hit.
	 */
	AttackCone Player::GetAttackCone();

	/**
	 * Makes the player hit another Movable with his attack.
	 * 
	 * Overriden so we can play some sounds.
	 * \param movable The Movable to attack.
	 */
	void Attack(Movable& movable);

	/**
	 * Makes the player take damage from another attack.
	 *
	 * Overriden so we can play a sound.
	 * \param amount The amount of health to remove.
	 */
	void RemoveHealth(int amount);

	/**
	 * Updates the player's position.
	 * This function should be called once per frame.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);

	/**
	 * Updates the HUD object.
	 * This function should be called once per frame.
	 */
	void UpdateHud(HUD& hud);

	/**
	 * Toggles the player's lighter on/off state
	 */
	void ToggleLighter();

	/**
	 * Adds a key to the player's inventory.
	 * \param doorName The name of the door in the level that the key should open.
	 */
	void AddKey(const std::string& doorName);
	/**
	 * Checks if the player has a key for a certain door.
	 * \param doorName The name of the door corresponding to the key.
	 * \return True if the player has the key, false otherwise.
	 */
	bool HasKey(const std::string& doorName);

	/**
	 * Adds a soul to the player's inventory.
	 */
	void AddSoul();

	/**
	 * Gets how many souls the player still has to collect to win.
	 * \return The number of souls the player still needs.
	 */
	int GetRemainingSouls();

	/**
	 * Resets the player, clearing all souls, keys etc. he has.
	 */
	void Reset();

//Private attributes; visible only within this class
private:

	enum {
		CANMOVE,
		ATTACKING
	} state;

	/** Flags set to true when the player should move in the corresponding direction */
	bool moveLeft, moveRight, moveUp, moveDown;

	/** Player's light source */
	LightSource lighter;
	/** Player's light source when the lighter is off */
	LightSource ambientLight;

	/** True when the player is sprinting, false when he is walking at regSpeed */
	bool isSprinting;

	/** Amount of fuel the player has left */
	sf::Time sprintTimer;
	/** Amount of fatigue the player has - directly proportional to the time he can run. */
	sf::Time lighterTimer;
	/** Amount of humanity the player still has - higher is better. */
	sf::Time humanityTimer;
	/** Number of pieces of soul the player has. */
	int souls;

	/** Names of doors the player can open. */
	std::set<std::string> doorNameSet;

	/** Timer for states */
	sf::Time timer;

	/** Time-to-live for any attack the player initiates. */
	static const sf::Time attackConeLife;
	/** Max burst sprinting time */
	static const sf::Time maxSprintTime;
	/** Amount of time the lighter can be on, from 100 fuel to 0 */
	static const sf::Time maxLighterTime;
	/** Amount of time before the player loses due to humanity */
	static const sf::Time maxHumanityTime;
	/** Length of the player's attack cone */
	static const float attackConeLength;
	/** Angle of the player's attack cone */
	static const float attackConeSweep;
	/** Regular walking speed, in pixels per second. */
	static const float regSpeed;
	/** Running speed, in pixels per second. */
	static const float sprintSpeed;
	/** Amount of humanity the player gains by picking up a soul. */
	static const float humanityIncrease;
	/** Amount of humanity the player loses by attacking. */
	static const float humanityDecrease;
	/** Amount of health the player gains by picking up a soul. */
	static const int healthIncrease;
	/** Number of souls the player needs to win. */
	static const int maxSouls;
};

#endif