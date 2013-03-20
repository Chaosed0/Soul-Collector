
#include "Torch.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Torch::Torch(const sf::Vector2f& pos)
	: Activatable("assets/img/Torch.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
	, light(128, 128, sf::Color(0,0,0,10), pos)
{
	SetPos(pos);
	light.SetPos(pos);

	AddAnimSet("on", 1, 7, true);
}

void Torch::Update(const Level& level, const sf::Time& timePassed)
{
	light.Update(level);
	if(IsActive() && !IsFinished()) {
		light.Toggle();
		isActive = false;
	}

	if(light.GetIsOn()) {
		PlayAnim("on", timePassed);
	} else {
		PlayAnim("idle", timePassed);
	}
}

void Torch::AddLight(Level& level)
{
	level.AddLight(light);
}