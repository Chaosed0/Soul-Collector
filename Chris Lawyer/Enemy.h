#ifndef __ENEMY_H
#define __ENEMY_H


#include <SFML/Graphics.hpp>

class Enemy: public sf::Drawable
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	float moveSpeed;
	int randomNumber;
	float xAxisLeft;
	float xAxisRight;
	float yAxisUp;
	float yAxisDown;

public:
	Enemy(std::string image, int width, int height, float speed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void autoRun(int width, int height);
	void randChange();
	
	float getXAxisLeft()
	{return xAxisLeft;}
	
	float getXAxisRight()
	{return xAxisRight;}
	
	float getYAxisUp()
	{return yAxisUp;}
	
	float getYAxisDown()
	{return yAxisDown;}
};

#endif