#include "Player.h"

Player::Player(string fileLocation)
{
	texture.loadFromFile(fileLocation);
	sprite.setTexture(texture);

}

void Player::move()
{
	

}

void Player::faster()
{

}

void Player::slower()
{

}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);

}