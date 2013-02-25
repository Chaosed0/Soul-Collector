
#include "Key.h"
#include "Level.h"

Key::Key(sf::Vector2f pos)
	: Entity("assets/img/Key.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	SetPos(pos);
}

void Key::Update(const Level& level)
{
	if(active) {
		active = false;
		visible = false;
		expended = true;
	}
}