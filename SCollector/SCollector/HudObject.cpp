#include "HudObject.h"

HudObject::HudObject()
{}

HudObject::HudObject(std::string image, float xPos, int winHeight)
{
	filled = 100;
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	sprite.setOrigin(0,sprite.getTextureRect().height/2);
	sprite.setPosition(xPos,winHeight-sprite.getTextureRect().height/2);
	rect.setFillColor(sf::Color::Green);
	rect.setOrigin(sprite.getOrigin());
	rect.setPosition(sprite.getPosition());
	rect.setSize(sf::Vector2f(sprite.getTextureRect().height,sprite.getTextureRect().width));
}

float HudObject::getWidth()
{
	return sprite.getTextureRect().width;
}

void HudObject::changeFill(float delta)
{
	if (rect.getSize().y + delta <= sprite.getTextureRect().height || rect.getSize().y + delta >= 0)
		rect.setSize(sf::Vector2f(rect.getTextureRect().width,rect.getTextureRect().height+delta));
	else if (delta > 0)
		rect.setSize(sf::Vector2f(rect.getTextureRect().width,rect.getTextureRect().height));
	else
		rect.setSize(sf::Vector2f(rect.getTextureRect().width,0));
		
}

void HudObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}