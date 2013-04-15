#include "HUD.h"

HUD::HUD(const std::vector<std::string>& names, int winHeight)
{
	float xPos = 0;
	for (int i = 0; i < numObjects; i++)
	{
		HudObject *object = new HudObject(names[i], xPos, winHeight);
		printf("Pushing object %d\n",i+1);
		objects.push_back(object);
		printf("Object %d has been pushed\n",i+1);
		xPos += objects.back()->getWidth();
	}
	printf("HUD constructor complete\n");

}

void HUD::changeFill(float delta, int which)
{
	(*objects[which]).changeFill(delta);
}
	
void HUD::draw(sf::RenderWindow &window) const
{
	for (int i = 0; i < numObjects; i++)
		window.draw(*objects[i]);

}
