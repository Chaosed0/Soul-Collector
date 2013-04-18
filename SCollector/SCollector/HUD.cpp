#include "HUD.h"

const sf::Time HUD::textDisplayTime = sf::seconds(3);

HUD::HUD(const sf::Vector2f& pos) :
	pos(pos) ,
	font()
{
	curXPos = 0;
	font.loadFromFile("assets/font/monkey.ttf");
	textObject.setFont(font);
	textObject.setCharacterSize(20);
}

void HUD::AddObject(const std::string& imgLoc)
{
	HudObject *object = new HudObject(imgLoc, sf::Vector2f(curXPos, pos.y));
	objects.push_back(object);
	curXPos += objects.back()->getWidth();
	textObject.setPosition(0,pos.y-objects[0]->getHeight()-textObject.getCharacterSize());
}

void HUD::changeFill(int final, int which)
{
	objects[which]->changeFill(final);
}

void HUD::changeText(std::string text)
{
	clock.restart();
	textObject.setString(text);
}
	
void HUD::draw(sf::RenderWindow &window) const
{
	for (unsigned int i = 0; i < objects.size(); i++)
		window.draw(*objects[i]);

	if (clock.getElapsedTime() < textDisplayTime)
		window.draw(textObject);
}
