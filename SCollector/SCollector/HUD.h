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

public:

	HUD(const std::vector<std::string>& names, int winHeight);

	void changeFill(float delta, int which);

	void draw(sf::RenderWindow& window) const;
};

#endif