//Implementation of Player class

#include "Player.h"

Player::Player(sf::Vector2f pos)
{
	Init(pos);
}

Player::Player(sf::Vector2f pos, std::string pathToImage)
{
	Init(pos);
	LoadTexture(pathToImage);
}

void Player::LoadTexture(std::string pathToImage)
{
	texture.loadFromFile(pathToImage);
	sprite.setTexture(texture);
	//Make the center of the player the place where all transformations take place
	sprite.setOrigin(sprite.getTextureRect().width/2, sprite.getTextureRect().height/2);
}

void Player::Init(sf::Vector2f pos)
{
	//When the player starts, he isn't moving anywhere
	moveLeft = moveRight = moveUp = moveDown = false;
	SetPos(pos);

	//ARGH CONSTANTS IT BURNS
	moveSpeed = 2.0f;
}

void Player::MoveLeft(bool start)
{
	moveLeft = start;
}
void Player::MoveRight(bool start)
{
	moveRight = start;
}
void Player::MoveUp(bool start)
{
	moveUp = start;
}
void Player::MoveDown(bool start)
{
	moveDown = start;
}

void Player::SetPos(sf::Vector2f newpos)
{
	sprite.setPosition(newpos);
}

void Player::Update(const Level& level)
{
	//Get the sides of the player's collision rectangle
	sf::Vector2f pos = sprite.getPosition();
	float top = pos.y - sprite.getTextureRect().height/2;
	float left = pos.x - sprite.getTextureRect().width/2;
	float bot = pos.y + sprite.getTextureRect().height/2;
	float right = pos.x + sprite.getTextureRect().width/2;
	sf::Vector2f topLeft(left, top);
	sf::Vector2f botRight(right, bot);
	sf::Vector2f topRight(right, top);
	sf::Vector2f botLeft(left, bot);
	//printf("(%g, %g), (%g, %g)\n", topLeft.x, topLeft.y, botRight.x, botRight.y);
	int nearestTopLeft, nearestBotRight, nearest;
	bool foundTopLeft, foundBotRight, found;

	//Level is needed here to grab collision information

	if(moveUp) {
		foundTopLeft = level.GetCollide(topLeft, false, false, nearestTopLeft);
		foundBotRight = level.GetCollide(topRight, false, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= top;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Up: %d\n", nearest);
		sprite.move(0, std::max(-moveSpeed, (float)nearest));
	}
	if(moveDown) {
		foundTopLeft = level.GetCollide(botLeft, false, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, false, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= bot;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Down: %d\n", nearest);
		sprite.move(0, std::min(moveSpeed, (float)nearest));
	}
	if(moveLeft) {
		foundTopLeft = level.GetCollide(topLeft, true, false, nearestTopLeft);
		foundBotRight = level.GetCollide(botLeft, true, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= left;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Left: %d\n", nearest);
		sprite.move(std::max(-moveSpeed, (float)nearest), 0);
	}
	if(moveRight) {
		foundTopLeft = level.GetCollide(topRight, true, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, true, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= right;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Right: %d\n", nearest);
		sprite.move(std::min(moveSpeed, (float)nearest), 0);
	}
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}