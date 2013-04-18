#include "HUD.h"

const sf::Time HUD::textDisplayTime = sf::seconds(3);

HUD::HUD(const std::vector<std::string>& names, int winHeight)
{
	float xPos = 0;
	for (int i = 0; i < numObjects; i++)
	{
		HudObject *object = new HudObject(names[i], xPos, winHeight);
		objects.push_back(object);
		xPos += objects.back()->getWidth();
	}


	textObject.setPosition(0,winHeight-objects[0]->getHeight()-textObject.getCharacterSize());
}

void HUD::changeFill(int final, int which)
{
	(*objects[which]).changeFill(final);
}

void HUD::changeText(std::string text)
{
	clock.restart();
	textObject.setString(text);
}
	
void HUD::draw(sf::RenderWindow &window) const
{
	for (int i = 0; i < numObjects; i++)
		window.draw(*objects[i]);

	if (clock.getElapsedTime() < textDisplayTime)
		window.draw(textObject);
}
