#include "HUD.h"

const sf::Time HUD::textDisplayTime = sf::seconds(3);
const int HUD::textPadding = 5;
const int HUD::humanityIncrement = 10;

HUD::HUD(const sf::Vector2f& pos) :
	pos(pos) ,
	font()
{
	curXPos = 0;
	prevHumanityChange = 100.0f;
	font.loadFromFile("assets/font/monkey.ttf");
	textObject.setFont(font);
	textObject.setCharacterSize(20);
}

void HUD::AddObject(const std::string& imgLoc)
{
	HudObject *object = new HudObject(imgLoc, sf::Vector2f(curXPos, pos.y));
	objects.push_back(object);
	curXPos += objects.back()->getWidth();
	textObject.setPosition(0,pos.y-textPadding-objects[0]->getHeight()-textObject.getCharacterSize());
}

void HUD::changeFill(int final, int which)
{
	objects[which]->changeFill(final);
	//If we're changing the humanity, we may also want to change the picture
	if(which == humanity && abs(prevHumanityChange - final) > humanityIncrement) {
		int imgIndex = (90-final)/10;
		char imgLoc[33];
		snprintf(imgLoc, 33, "assets/img/hud_soul_beast0%d0.png", imgIndex);
		objects[which]->SetImage(imgLoc);
	}
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
