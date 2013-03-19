#include "HUD.h"

HUD::HUD(const std::vector<std::string>& names, int winHeight)
{
	float xPos = 0;
	for (int i = 0; i < names.size(); i++)
	{
		printf("Pushing object %d\n",i+1);
		objects.push_back(HudObject(names[i], xPos, winHeight));
		printf("Object %d has been pushed\n",i+1);
		xPos += objects.back().getWidth();
	}
	printf("HUD constructor complete\n");
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < numObjects; i++)
		target.draw(objects[i], states);

}
