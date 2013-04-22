/**
 * \brief Represents a transition to another level.
 * \author Ed Lu
 * \date March 21 2013
 */

#include <string>

#include "Activatable.h"
class Level;

class Stairs : public Activatable
{
public:
	/**
	 * Default constructor.
	 * \param pos The position of the stairs within the level.
	 */
	Stairs(const sf::Vector2f& pos, const std::string& nextLevel, const std::string& nextSpawn);

	/**
	 * Update function.
	 * \param level The level in which the key is located.
	 * \param timePassed time passed since the last time the function was called.
	 */
	virtual void Update(Level& level, const sf::Time& timePassed);

	/**
	 * Gets the name of the next level the stairs go to.
	 * \return The name of the next level.
	 */
	std::string GetNextLevel();

	/**
	 * Gets the name of the spawn point in the next level the stairs go to.
	 * \return The name of the spawn point.
	 */
	std::string GetNextSpawn();
	
private:
	std::string nextLevel;
	std::string nextSpawn;
};
