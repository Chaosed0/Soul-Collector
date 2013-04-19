#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "HudObject.h"

class HUD
{
public:
	HUD(const sf::Vector2f& pos);

	/**
	 * Adds a HUDObject to the HUD.
	 * \param imgLoc The location of the image to use in the HUDObject.
	 */
	void AddObject(const std::string& imgLoc);

	void changeFill(int final, int which);
	void changeText(std::string text);

	void draw(sf::RenderWindow& window) const;

	static const enum {
		health = 0,
		fuel,
		humanity,
		fatigue
	};
private:
	std::vector<HudObject *> objects;

	sf::Font font;
	sf::Text textObject;
	sf::Clock clock;

	float curXPos;
	float prevHumanityChange;
	sf::Vector2f pos;

	static const sf::Time textDisplayTime;
	static const int textPadding;
	static const int humanityIncrement;
};

#endif