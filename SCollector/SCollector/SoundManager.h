/**
 * \brief Manages sounds for entities.
 * \author Ed Lu
 * \date March 21 2013
 */

#ifndef __SOUNDMANAGER_H
#define __SOUNDMANAGER_H

#include <map>
#include <vector>

#include <SFML/Audio.hpp>

class SoundManager
{
public:
	/**
	 * Constructor.
	 */
	SoundManager();

	/**
	 * Adds a sound to the library.
	 * \param soundLoc The location, on disk, of the sound.
	 * \param soundName The name to associate with the sound.
	 * \param loop Flag indicating whether the sound should loop or not; true for yes.
	 * \return True of the sound was successfully added, false otherwise.
	 */
	bool AddSound(const std::string& soundLoc, const std::string& soundName, bool loop); 

	/**
	 * Plays a sound.
	 * \param soundName The name of the sound to play.
	 * \return True if the sound exists, false otherwise.
	 */
	bool PlaySound(const std::string& soundName);

	/**
	 * Stops playing a sound.
	 * \param soundName The name of the sound to play.
	 * \return True if the sound exists, false otherwise.
	 */
	bool StopSound(const std::string& soundName);

	/**
	 * Stops playing the last sound played.
	 */
	void StopLastSound();
private:
	/** Map of sound names to sound identifiers. */
	std::map<std::string, int> soundNames;
	/** Vector of sounds associated with the sound buffers. */
	std::vector<sf::Sound> sounds;
	/** Vector of flags indicating whether the sounds should loop or not. */
	std::vector<bool> soundLoop;

	/** The last sound identifier used in the map. */
	int lastSoundAdded;
	/** The last sound identifier played. */
	int lastSoundPlayed;

	/** Map of sound locations to sound buffers - if a buffer is
	 *  re-used between sound managers, then it doesn't get re-loaded */
	static std::map<std::string, sf::SoundBuffer> soundBuffers;
};

#endif