#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "HudObject.h"

class HUD
{
private:

	static const int numObjects = 3;
	std::vector<HudObject *> objects;
	sf::Text textObject;
	sf::Clock clock;
	static const sf::Time textDisplayTime;

public:

	HUD(const std::vector<std::string>& names, int winHeight);

	void changeFill(int final, int which);
	void changeText(std::string text);

	void draw(sf::RenderWindow& window) const;
};

#endif