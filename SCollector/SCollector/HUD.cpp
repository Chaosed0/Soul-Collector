#include "HUD.h"

HUD::HUD(std::string names[numObjects],int winHeight)
{
	float xPos = 0;
	for (int i = 0; i < numObjects; i++)
	{
		HudObject object(names[i],xPos,winHeight);
		xPos += object.getWidth();
		objects[i] = object;
	}

}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < numObjects; i++)
		target.draw(objects[i], states);

}
