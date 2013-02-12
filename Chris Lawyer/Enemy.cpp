#include "Enemy.h"

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(sprite,state);
}

Enemy::Enemy(std::string image, int width, int height, float speed)
{
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	int spriteWidth = sprite.getTextureRect().width;
	int spriteHeight = sprite.getTextureRect().height;
	sprite.setOrigin(spriteWidth/2,spriteHeight/2);
	sprite.setPosition(rand() % width,rand() % height);
	moveSpeed = speed;
	randomNumber = rand() % 4;
	xAxisLeft = sprite.getPosition().x - spriteWidth/2;
	xAxisRight = sprite.getPosition().x + spriteWidth/2;
	yAxisUp = sprite.getPosition().y - spriteHeight/2;
	yAxisDown = sprite.getPosition().y + spriteHeight/2;
}

void Enemy::autoRun(int winWidth, int winHeight)
{
	float xPos = sprite.getPosition().x;
	float yPos = sprite.getPosition().y;
	int spriteHeight = sprite.getTextureRect().height;
	int spriteWidth = sprite.getTextureRect().width;
	int changeCheck = rand() % 1000;
	int temp;

	if (changeCheck == 0)
		randChange();

	switch(randomNumber)
	{
	case 0:
		if (yPos - spriteHeight/2 > 0)
		{
			sprite.move(0, -moveSpeed);
			yAxisUp -= moveSpeed;
			yAxisDown -= moveSpeed;
		}
		else
			randChange();
		break;
	case 1:
		if (xPos - spriteWidth/2 > 0)
		{
			sprite.move(-moveSpeed, 0);
			xAxisLeft -= moveSpeed;
			xAxisRight -= moveSpeed;
		}
		else
			randChange();
		break;
	case 2:
		if (yPos + spriteHeight/2 < winHeight)
		{
			sprite.move(0, moveSpeed);
			yAxisUp += moveSpeed;
			yAxisDown += moveSpeed;
		}
		else
			randChange();
		break;
	case 3:
		if (xPos + spriteWidth/2 < winWidth)
		{
			sprite.move(moveSpeed, 0);
			xAxisLeft += moveSpeed;
			xAxisRight += moveSpeed;
		}
		else
			randChange();
		break;
	}
}

void Enemy::randChange()
{
	int temp = randomNumber;

	do
		randomNumber = rand() % 4;
	while (randomNumber == temp);
}

