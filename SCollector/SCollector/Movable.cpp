
#include "Movable.h"

Movable::Movable(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: Entity(imgLoc, collisionBox, animBox)
{
	attackPower = 25;
	health = 100;
}