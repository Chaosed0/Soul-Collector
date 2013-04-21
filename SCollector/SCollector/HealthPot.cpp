/**
 * \brief A potion that cures some of the player's health.
 * \author Edward Lu
 * \date April 19 2013
 */

#include "HealthPot.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

HealthPot::HealthPot(const sf::Vector2f& pos, int recovery)
	: Activatable("assets/img/HealthPot.png", sf::IntRect(0,0,32,32),
		sf::IntRect(0,0,32,32), sf::IntRect(-8,-8,48,48))
{
	SetPos(pos);
	this->recovery = recovery;
	soundManager.AddSound("assets/sound/pickup.ogg", "pickup", false);
}

void HealthPot::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		level.GetPlayer().AddHealth(recovery);
		char message[50];
		sprintf_s(message, 50, "Recovered %d health.", recovery);
		level.SetHUDText(message);
		soundManager.PlaySound("pickup");
		Finish();
	}

	//Just play the idle animation to keep the clock in sync
	PlayAnim("idle", timePassed);
}