#include "HudObject.h"

HudObject::HudObject()
{}

HudObject::HudObject(std::string image, float xPos, int winHeight)
{
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	sprite.setOrigin(0,sprite.getTextureRect().height/2);
	sprite.setPosition(xPos,winHeight-sprite.getTextureRect().height/2);
	rect.setFillColor(sf::Color::Red);
	rect.setOrigin(sprite.getOrigin());
	rect.setPosition(sprite.getPosition());
	rect.setSize(sf::Vector2f(sprite.getTextureRect().width,sprite.getTextureRect().height));
}

float HudObject::getWidth()
{
	return sprite.getTextureRect().width;
}

void HudObject::changeFill(float delta)
{
	if (rect.getSize().y + delta <= sprite.getTextureRect().height && rect.getSize().y + delta >= 0)
	{
		//printf("Changing fill..\n");
		rect.setSize(sf::Vector2f(rect.getSize().x,rect.getSize().y+delta));
		printf("Height is now %d\n",rect.getTextureRect().height);
	}
	else if (delta > 0)
	{
		//printf("Full\n");
		rect.setSize(sf::Vector2f(rect.getSize().x,sprite.getTextureRect().height));
	}
	else
	{
		//printf("Empty\n");
		rect.setSize(sf::Vector2f(rect.getSize().x,0));
	}
}

void HudObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect,states);
	target.draw(sprite, states);
}