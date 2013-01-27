#ifndef PLAYER_H
#define PLAYER_H

#include <sfml/Graphics.hpp>
using namespace std;

class Player: public sf::Drawable
{
private: 
	// player attributes go here
	sf::Sprite sprite;
	sf::Texture texture;

public:
	Player(string fileLocation);

	void move();	// moves the player & bounces off screen
	void faster();	// makes speed faster
	void slower();	// makes speed slower

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};





#endif