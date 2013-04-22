
#include "Activatable.h"

Activatable::Activatable(std::string imgLoc, sf::IntRect collisionBox,
	sf::IntRect animBox, sf::IntRect activateBox)
	: Entity(imgLoc, collisionBox, animBox, false)
{
	isActive = false;
	isFinished = false;
	this->activateBox = activateBox;
}

bool Activatable::TryActivate(const Entity& entity)
{
	sf::IntRect box = activateBox;
	box.left += (int)(GetPos().x - sprite.getOrigin().x);
	box.top += (int)(GetPos().y - sprite.getOrigin().y);
	if(entity.IsColliding(box)) {
		Activate();
		return true;
	}
	return false;
}

void Activatable::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	Entity::draw(target, state);

#if DRAW_ACTCOLBOXES == 1 
	sf::IntRect box = activateBox;
	box.left += (int)GetPos().x - sprite.getOrigin().x;
	box.top += (int)GetPos().y - sprite.getOrigin().y;
	sf::RectangleShape rect(sf::Vector2f(box.width, box.height));
	rect.setPosition(box.left, box.top);
	rect.setOutlineColor(sf::Color(255, 0, 0, 255));
	rect.setFillColor(sf::Color(0,0,0,0));
	rect.setOutlineThickness(1.0f);
	target.draw(rect, state);
#endif
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
