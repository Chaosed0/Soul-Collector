#include "HudObject.h"

HudObject::HudObject()
{}

HudObject::HudObject(std::string image, float xPos, int winHeight)
{
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	sprite.setOrigin(0,sprite.getTextureRect().height/2);
	sprite.setPosition(xPos,winHeight-sprite.getTextureRect().height/2);
}

float HudObject::getWidth()
{
	return sprite.getTextureRect.width();
}

void HudObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}