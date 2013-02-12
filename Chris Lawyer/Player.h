
#ifndef __PLAYER_H
#define __PLAYER_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Player:public sf::Drawable
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	float moveSpeed;
	float xAxisLeft;
	float xAxisRight;
	float yAxisUp;
	float yAxisDown;

public:
	Player(std::string image, float width, float height, float speed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void move(bool up, bool left, bool down, bool right, int width, int height);
	bool collisionCheck(Enemy enemy);
	void die();
	
};

#endif