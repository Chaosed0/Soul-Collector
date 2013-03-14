
#include "Key.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Key::Key(const sf::Vector2f& pos)
	: Activatable("assets/img/Key.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	SetPos(pos);
}

void Key::Update(const Level& level)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		Finish();
	}
}