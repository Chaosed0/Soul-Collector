
#include "Torch.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

Torch::Torch(const sf::Vector2f& pos)
	: Activatable("assets/img/Torch.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
	, light(128, 128, sf::Color(), pos)
{
	SetPos(pos);
	light.SetPos(pos);
}

void Torch::Update(const Level& level)
{
	light.Update(level);
	if(IsActive() && !IsFinished()) {
		light.Toggle();
		isActive = false;
	}
}

void Torch::AddLight(Level& level)
{
	level.AddLight(light);
}