
#include "Entity.h"
#include "Level.h"

Entity::Entity(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: animManager(animBox)
{
	//Set default values
	this->collisionBox = collisionBox;
	visible = true;

	//Load the texture
	texture.loadFromFile(imgLoc);
	sprite.setTexture(texture);
	sprite.setOrigin(animBox.width/2.0f, animBox.height/2.0f);

	//Now that we have the texture, initialize the animation manager with it
	animManager.SetSheetSize(texture.getSize());

	sprite.setTextureRect(animManager.GetCurAnimRect());
}

bool Entity::IsColliding(const sf::IntRect& box) const
{
	//Separating Axis Theorem: Two boxes are colliding if and only if they are colliding
	// on their component axes (x and y)
	sf::IntRect thisCollisionBox = collisionBox;
	thisCollisionBox.left = (int)GetPos().x;
	thisCollisionBox.top = (int)GetPos().y;

	//Left side of the other box in this one
	bool leftCollide =
		box.left >= thisCollisionBox.left &&
		box.left <= thisCollisionBox.left + thisCollisionBox.width;
	//Right side of other box in this one
	bool rightCollide = 
		box.left + box.width >= thisCollisionBox.left &&
		box.left + box.width <= thisCollisionBox.left + thisCollisionBox.width;
	//Top side of the other box in this one
	bool topCollide = 
		box.top >= thisCollisionBox.top &&
		box.top <= thisCollisionBox.top + thisCollisionBox.height;
	//Bottom side of the other box in this one
	bool botCollide = 
		box.top + box.height >= thisCollisionBox.top &&
		box.top + box.height <= thisCollisionBox.top + thisCollisionBox.height;

	//printf("%d, %d, %d, %d\n", leftCollide, rightCollide, topCollide, botCollide);
	//printf("%d, %d, %d, %d\n", entity.collisionBox.left, entity.collisionBox.width, collisionBox.left, collisionBox.width);

	if((leftCollide || rightCollide) && (topCollide || botCollide))
		return true;
	return false;
}

bool Entity::IsColliding(const Entity& entity) const
{
	//Separating Axis Theorem: Two boxes are colliding if and only if they are colliding
	// on their component axes (x and y)
	sf::IntRect entCollisionBox  = entity.collisionBox;
	entCollisionBox.left = (int)entity.GetPos().x;
	entCollisionBox.top = (int)entity.GetPos().y;
	return IsColliding(entCollisionBox);
}

bool Entity::IsColliding(const AttackCone& cone) const
{
	//The entity is colliding with the cone if any of its four corners are within
	// the cone
	// (not strictly true, the cone might be inside the entity, but we ignore that possibility)
	sf::Vector2f topLeft(GetPos());
	sf::Vector2f topRight(topLeft.x+collisionBox.height, topLeft.y);
	sf::Vector2f botLeft(topLeft.x, topLeft.y+collisionBox.width);
	sf::Vector2f botRight(topRight.x, botLeft.y);

	return cone.Contains(topLeft) ||
		cone.Contains(topRight) ||
		cone.Contains(botLeft) ||
		cone.Contains(botRight);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if(visible)
		target.draw(sprite, state);
}


void Entity::SetPos(sf::Vector2f newpos)
{
	sprite.setPosition(newpos);
}

sf::Vector2f Entity::GetPos() const
{
	return sprite.getPosition();
}

void Entity::SetRot(float newrot)
{
	sprite.setRotation(newrot);
	printf("Got rot of %g\n", newrot);
}

void Entity::PlayAnim(const std::string& anim, const sf::Time& timePassed)
{
	animManager.PlayAnim(anim, timePassed);
	sprite.setTextureRect(animManager.GetCurAnimRect());
}