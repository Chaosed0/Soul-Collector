/**
 * \brief An object within the HUD containing a sprite,texture, rectangle, and value
 * \author Chris Lawyer
 * \Date March 21 2013
 */

#ifndef HUDOBJECT_H
#define HUDOBJECT_H

#include <SFML/Graphics.hpp>

class HudObject:public sf::Drawable
{
private:

	//The texture of the object
	sf::Texture texture;
	//The sprite of the object
	sf::Sprite sprite;
	//The rectangle of the object used to show a filling bar
	sf::RectangleShape rect;
	//The value associated with the rectangle
	int value;

public:

	//Default constructor
	HudObject();

	/**
	 * Constructor
	 * \param image The image to be used as the sprite for the object
	 * \param pos The x position at which the object is to be placed
	 */
	HudObject(const std::string& image, const sf::Vector2f& pos);

	//Gets the width of the object
	float getWidth();
	//Gets the height of the object
	float getHeight();
	
	/**
	 * Changes the value associated with the object
	 * \param final The number to which the value is to be changed
	 */
	void changeFill(int final);

	/**
	 * Loads the image to the texture
	 * \param image The name of the image to be used
	 */
	void SetImage(const std::string& image);

	//draws the HudObject to the window
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif