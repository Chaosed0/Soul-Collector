#include "HudObject.h"

HudObject::HudObject()
{}

HudObject::HudObject(const std::string& image, const sf::Vector2f& pos)
{
	value = 100;
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	sprite.setOrigin(0,sprite.getTextureRect().height/2.0f);
	sprite.setPosition(pos.x + sprite.getOrigin().x, pos.y - sprite.getOrigin().y);
	rect.setFillColor(sf::Color::White);
	
	rect.setSize(sf::Vector2f((float)sprite.getTextureRect().width,(float)sprite.getTextureRect().height));
	rect.setPosition(pos.x,pos.y-rect.getSize().y);
}

float HudObject::getWidth()
{
	return (float)sprite.getTextureRect().width;
}

float HudObject::getHeight()
{
	return (float)sprite.getTextureRect().height;
}

void HudObject::SetImage(const std::string& image)
{
	texture.loadFromFile(image);
}

void HudObject::move(sf::Vector2f &movement) {
	rect.move(movement);
	sprite.move(movement);
}

void HudObject::changeFill(int final)
{
	int delta = final - value;
	value = final;

	rect.setSize(sf::Vector2f(rect.getSize().x,sprite.getTextureRect().height*value/100.0f));
	rect.setPosition(rect.getPosition().x,rect.getPosition().y-sprite.getTextureRect().height*delta/100.0f);
}

void HudObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect,states);
	target.draw(sprite, states);
}
