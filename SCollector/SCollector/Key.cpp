
#include "Key.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Key::Key(const sf::Vector2f& pos, std::string doorName)
	: Activatable("assets/img/Key.png", sf::IntRect(0,0,32,32),
		sf::IntRect(0,0,32,32), sf::IntRect(-16,-16,64,64))
{
	SetPos(pos);
	this->doorName = doorName;
}

void Key::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		level.GetPlayer().AddKey(doorName);
		Finish();
	}

	//Just play the idle animation to keep the clock in sync
	PlayAnim("idle", timePassed);
}