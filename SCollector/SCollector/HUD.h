#include <SFML/Graphics.hpp>

#include "HudObject.h"

class HUD:public sf::Drawable
{
private:

	static const int numObjects = 3;
	HudObject objects[numObjects];
	std::string imageNames[numObjects];

public:

	HUD(std::string names[numObjects], int winHeight);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
