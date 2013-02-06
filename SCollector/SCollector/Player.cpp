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
	sprite.setPosition(pos);

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

void Player::Update()
{
	// Remember from your graphics class; the window coordinates are like this:
	//      pos x =>
	//        ______
	//       |      |
	// pos|| |      |
	//  y \/ |      |
	//       |______|
	//
	// That means if we want the player to move up, we need to move the player in the
	//  NEGATIVE y direction!

	//Another thing to note here:
	// I don't use "else if" because if the player is pressing up and down at the same time,
	// then we don't want him to move anywhere.
	// Additionally, if the player is pressing both up and right, he'll move diagonally.
	if(moveUp)
		sprite.move(0, -moveSpeed);
	if(moveDown)
		sprite.move(0, moveSpeed);
	if(moveLeft)
		sprite.move(-moveSpeed, 0);
	if(moveRight)
		sprite.move(moveSpeed, 0);
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}