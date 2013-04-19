
#include "Soul.h"
#include "Level.h"
#include "Player.h"

Soul::Soul(const sf::Vector2f& pos)
	: Activatable("assets/img/Soul.png", sf::IntRect(0,0,32,32),
		sf::IntRect(0,0,32,32), sf::IntRect(-8,-8,48,48))
{
	SetPos(pos);
	animManager.ModifyAnimSet("idle", 0, 11, true);
	soundManager.AddSound("assets/sound/pickup.ogg", "pickup", false);
}

void Soul::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		level.GetPlayer().AddSoul();
		int soulsRemain = level.GetPlayer().GetRemainingSouls();
		char msg[60];
		sprintf_s(msg, 60, "Got a piece of your soul! %d remain...", soulsRemain);
		level.SetHUDText(msg);
		soundManager.PlaySound("pickup");
		Finish();
	}

	// Play the idle animation
	PlayAnim("idle", timePassed);
}