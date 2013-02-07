#include "Player.h"
#include "Enemy.h"

void Player::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(sprite,state);
}

Player::Player(std::string image, float width, float height, float speed)
{
	texture.loadFromFile(image);
	sprite.setTexture(texture);
	int spriteWidth = sprite.getTextureRect().width;
	int spriteHeight = sprite.getTextureRect().height;
	sprite.setOrigin(spriteWidth/2,spriteHeight/2);
	sprite.setPosition(width/2,height/2);
	moveSpeed = speed;
	xAxisLeft = sprite.getPosition().x - spriteWidth/2;
	xAxisRight = sprite.getPosition().x + spriteWidth/2;
	yAxisUp = sprite.getPosition().y - spriteHeight/2;
	yAxisDown = sprite.getPosition().y + spriteHeight/2;
}

void Player::move(bool up, bool left, bool down, bool right, int winWidth, int winHeight)
{
		float xPos = sprite.getPosition().x;
		float yPos = sprite.getPosition().y;
		float spriteHeight = sprite.getTextureRect().height;
		float spriteWidth = sprite.getTextureRect().width;

		if (up && yPos - spriteHeight/2 > 0)
		{
			sprite.move(0,-moveSpeed);
			yAxisUp -= moveSpeed;
			yAxisDown -= moveSpeed;
		}
		if (left && xPos - spriteWidth/2 > 0)
		{
			sprite.move(-moveSpeed,0);
			xAxisLeft -= moveSpeed;
			xAxisRight -= moveSpeed;
		}
		if (down && yPos + spriteHeight/2 < winHeight)
		{
			sprite.move(0,moveSpeed);
			yAxisUp += moveSpeed;
			yAxisDown += moveSpeed;
		}
		if (right && xPos + spriteWidth/2 < winWidth)
		{
			sprite.move(moveSpeed,0);
			xAxisLeft += moveSpeed;
			xAxisRight += moveSpeed;
		}
}

bool Player::collisionCheck(Enemy enemy)
{
	if (xAxisRight <= enemy.getXAxisLeft() || xAxisLeft >= enemy.getXAxisRight())
		return false;
	else
	{
		if (yAxisUp >= enemy.getYAxisDown() || yAxisDown <= enemy.getYAxisUp())
			return false;
		else
			return true;
	}
}