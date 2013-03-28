/**
 * \brief Manages Levels in the game.
 * \author Edward Lu
 * \date March 27 2013
 */

#ifndef __LEVELMANAGER_H
#define __LEVELMANAGER_H

#include "Level.h"
#include "Player.h"

#include <map>

class LevelManager
{
public:
	/** Default constructor. */
	LevelManager();
	/**
	 * Init constructor.
	 *
	 * Equivalent to calling LevelManager() and then LoadMap(mapName, spawnName).
	 * \param mapName The name of the map, on disk, to load.
	 * \param spawnName The name of the spawn within the map to start the player at.
	 */
	LevelManager(const std::string& mapName, const std::string& spawnName);

	/**
	 * Changes the current map.
	 *
	 * If the current map isn't already loaded or FromDisk == true,
	 *  this function will load the map from disk. Otherwise, it will
	 *  simply load the previous map.
	 * \param mapName The name of the map to load, from disk.
	 * \param spawnName The name of the spawn within the map to start the player at.
	 */
	bool LoadMap(const std::string& mapName, const std::string& spawnName);

	/**
	 * Gets the current Level.
	 * \return The current Level.
	 */
	Level& GetCurrentLevel();

	/**
	 * Updates the current map.
	 *
	 * Also checks for level transitions.
	 * \param timePassed The amount of time past since the last time Update() was called.
	 */
	bool Update(const sf::Time& timePassed);
private:
	/** List of levels */
	std::map<std::string, Level*> levels;
	/** Name of the current map. */
	std::string curLevelName;
	/** The actual player. */
	Player player;
};

#endif