#include "player.h"



player::player(string imageLocation)
{float x = 10,y= 15;
	texture.loadFromFile(imageLocation);
	sprite.setTexture(texture);
	sprite.setPosition(100,25);
	move(sprite.getPosition().x + x, sprite.getPosition().y + y);
	faster();
	slower();

}


player::~player(void)
{
}

 void player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);

}