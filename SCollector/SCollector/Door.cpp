#include "Door.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Door::Door()
	: Activatable("Door.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	health = 100.0f;
	isOpened = false;
	isBroken = false;
	isCollidable = true;
}

Door::Door(const sf::Vector2f& pos, std::string doorName)
	: Activatable(" ", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	this->doorName = doorName;
	health = 100.0f;
	isOpened = false;
	isBroken = false;
	SetPos(pos);
}

void Door::Break()
{
	if(this->health == 0)
		isBroken = true;
}

double Door::GetHealth() const
{
	return health;
}

void Door::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		if(level.GetPlayer().HasKey(doorName)) {
			isOpened = true;
			Finish();
		} else {
			isActive = false;
		}
		isCollidable = !isOpened && !isBroken;
	}
}