#pragma once

#include <SFML\Graphics.hpp>
using namespace std;

class player: public sf::Drawable
{

public:
	player(string imageLocation);
	~player(void);
	void move();

private:
	sf::Texture texture;
	sf::Sprite sprite;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

