#include "player.h"



player::player(string imageLocation)
{
	texture.loadFromFile(imageLocation);
	sprite.setTexture(texture);
	sprite.setPosition(100,25);
}


player::~player(void)
{
}

 void player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);

}