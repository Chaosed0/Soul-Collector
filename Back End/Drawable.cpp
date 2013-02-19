#include "Drawable.h"

Drawable::Drawable(string fileLocation)
{
	name = "";
	hasDrawn = false;
	isDiscovered = false;

	texture.loadFromFile(fileLocation);
	sprite.setTexture(texture);

}

void Drawable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}