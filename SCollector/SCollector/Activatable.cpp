
#include "Activatable.h"

Activatable::Activatable(std::string imgLoc, sf::IntRect collisionBox,
	sf::IntRect animBox, sf::IntRect activateBox)
	: Entity(imgLoc, collisionBox, animBox, false)
{
	isActive = false;
	isFinished = false;
	this->activateBox = activateBox;
}

void Activatable::Activate()
{
	isActive = true;
}

bool Activatable::IsActive()
{
	return isActive;
}

void Activatable::Finish()
{
	isFinished = true;
	isActive = false;
}

bool Activatable::IsFinished()
{
	return isFinished;
}