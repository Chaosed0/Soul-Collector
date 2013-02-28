
#include "Key.h"
#include "Entity.h"
#include "Level.h"

Key::Key(sf::Vector2f pos)
	: Activatable("assets/img/Key.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	SetPos(pos);
}

void Key::Update(const Level& level)	//finish set to false
{
	if(IsActive()) {
		Finish();
	}
}

void Key::pickup(Player player)		//if the player touch the key, pick it up and set visible to false
{
	if(player.IsColliding(*this) == true)
	{
		visible = false;
		myKeys.push_back (*this);
	}
}