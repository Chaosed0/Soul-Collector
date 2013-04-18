#ifndef HUDOBJECT_H
#define HUDOBJECT_H

#include <SFML/Graphics.hpp>

class HudObject:public sf::Drawable
{
private:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape rect;
	int value;

public:

	HudObject();
	HudObject(std::string image, float xPos, int winHeight);

	float getWidth();
	float getHeight();

	void changeFill(int final);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif