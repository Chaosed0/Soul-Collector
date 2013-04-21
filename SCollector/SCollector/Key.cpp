
#include "Key.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Key::Key(const sf::Vector2f& pos, std::string doorName, const std::string& description)
	: Activatable("assets/img/Key.png", sf::IntRect(0,0,32,32),
		sf::IntRect(0,0,32,32), sf::IntRect(-8,-8,48,48))
{
	SetPos(pos);
	this->doorName = doorName;
	this->description = description;
	soundManager.AddSound("assets/sound/pickup.ogg", "pickup", false);
}

void Key::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		level.GetPlayer().AddKey(doorName);
		level.SetHUDText("Got " + description + ".");
		soundManager.PlaySound("pickup");
		Finish();
	}

	//Just play the idle animation to keep the clock in sync
	PlayAnim("idle", timePassed);
}