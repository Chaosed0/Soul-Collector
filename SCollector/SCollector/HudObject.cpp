#include "HudObject.h"

HudObject::HudObject()
{}

HudObject::HudObject(const std::string& image, const sf::Vector2f& pos)
{
	value = 100;
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	sprite.setOrigin(0,sprite.getTextureRect().height/2);
	sprite.setPosition(pos.x + sprite.getOrigin().x, pos.y - sprite.getOrigin().y);
	rect.setFillColor(sf::Color::Red);
	
	rect.setSize(sf::Vector2f(sprite.getTextureRect().width,sprite.getTextureRect().height));
	rect.setPosition(pos.x,pos.y-rect.getSize().y);
}

float HudObject::getWidth()
{
	return sprite.getTextureRect().width;
}

float HudObject::getHeight()
{
	return sprite.getTextureRect().height;
}

void HudObject::changeFill(int final)
{
	int delta = final - value;
	value = final;

	rect.setSize(sf::Vector2f(rect.getSize().x,sprite.getTextureRect().height*value/100.0));
	rect.setPosition(rect.getPosition().x,rect.getPosition().y-sprite.getTextureRect().height*delta/100.0);
}

void HudObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect,states);
	target.draw(sprite, states);
}