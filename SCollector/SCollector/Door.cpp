#include "Door.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Door::Door(const sf::Vector2f& pos, const std::string& doorName, const std::string& description)
	: Activatable("assets/img/Door.png", sf::IntRect(0,0,32,32),
		sf::IntRect(0,0,32,32), sf::IntRect(-8,-8,48,48))
{
	this->doorName = doorName;
	this->description = description;
	health = 100;
	isOpened = false;
	isBroken = false;
	isCollidable = true;
	animManager.AddAnimSet("open", 1, 1, true);
	animManager.AddAnimSet("broken", 2, 2, true);
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
		if(level.GetPlayer().HasKey(doorName)) {
			level.SetHUDText("Used " + description + ".");
			isOpened = true;
			Finish();
		} else {
			level.SetHUDText("Locked; need " + description + ".");
			isActive = false;
		}
		isCollidable = !isOpened && !isBroken;
	}

	if(isOpened) {
		PlayAnim("open", timePassed);
	} else if(isBroken) {
		PlayAnim("broken", timePassed);
	} else {
		PlayAnim("idle", timePassed);
	}
}