
#include "Key.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Key::Key(const sf::Vector2f& pos)
	: Activatable("assets/img/Key.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	SetPos(pos);
}

void Key::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		Finish();
	}

	//Just play the idle animation to keep the clock in sync
	PlayAnim("idle", timePassed);
}

int Key::getKeyID()
{
	return KeyID;
}

void Key::setKeyID(int keyID)
{
	KeyID = keyID;
}