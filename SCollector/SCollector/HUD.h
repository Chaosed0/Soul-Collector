/**
 * \brief The HUD, which contains HudObjects and a text string
 * \author Chris Lawyer
 * \Date March 21 2013
 */

#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "HudObject.h"

class HUD
{
public:
	/**
	 * Constructor.
	 * \param pos The position of the BOTTOM-LEFT of the HUD.
	 */
	HUD(const sf::Vector2f& pos);

	/**
	 * Updates the HUD's position.
	 * \param pos The position of the BOTTOM-LEFT of the HUD.
	 **/
	void Reposition(const sf::Vector2f &pos);

	/**
	 * Adds a HUDObject to the HUD.
	 * \param imgLoc The location of the image to use in the HUDObject.
	 */
	void AddObject(const std::string& imgLoc);

	/**
	 * Changes the fullness of the bar for a single HudObject
	 * \param final The target final value of the bar (0-100)
	 * \param which Indicator as to which HudObject to change (0 is the leftmost object)
	 */
	void changeFill(int final, int which);

	/**
	 * Changes the text of the HUD's textObject
	 * \param text The new value of the text of textObject
	 */
	void changeText(std::string text);

	/**
	 * Draws the HUD to the window as well as its HudObjects and textObject
	 * \param window The window to be drawn to
	 */
	void draw(sf::RenderWindow& window) const;

	/**
	 * The values associated with the HudObjects
	 */
	enum {
		health = 0,
		fuel,
		humanity,
		fatigue
	};
private:
	//Vector of HudObject pointers to point to the HudObjects in the HUD
	std::vector<HudObject *> objects;

	//The position in vector form 
	sf::Vector2f pos;

	//The font used for the textObject
	sf::Font font;
	//Text to be displayed on the screen above the HUD
	sf::Text textObject;
	//Clock to keep track of time for the textObject
	sf::Clock clock;

	//The current x position to draw HudObjects side-by-side
	float curXPos;
	//Value to keep track of previous humanity changes
	float prevHumanityChange;

	//The amount of time text should be displayed on the screen by the textObject
	static const sf::Time textDisplayTime;
	//The distance in pixels between the text for the textObject and the HudObjects
	static const int textPadding;
	//The value at which the humanity object will change pictures
	static const int humanityIncrement;
};

#endif
