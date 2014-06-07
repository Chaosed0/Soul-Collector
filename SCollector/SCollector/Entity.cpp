
#include "Entity.h"
#include "Level.h"

std::map<std::string, sf::Texture> Entity::textures;

Entity::Entity(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox, bool isCollidable)
	: animManager(animBox)
{
	//Set default values
	this->collisionBox = collisionBox;
	this->isCollidable = isCollidable;
	visible = true;

	//Load the texture
	sprite.setTexture(GetTexture(imgLoc));
	sprite.setOrigin(animBox.width/2.0f, animBox.height/2.0f);

	//Now that we have the texture, initialize the animation manager with it
	animManager.SetSheetSize(sprite.getTexture()->getSize());

	sprite.setTextureRect(animManager.GetCurAnimRect());
}

sf::Texture& Entity::GetTexture(const std::string& imgLoc)
{
	if(textures.find(imgLoc) == textures.end()) {
		sf::Texture texture;
		if(!texture.loadFromFile(imgLoc)) {
			fprintf(stderr, "ERROR: Could not load texture from file %s!\n", imgLoc.c_str());
		}
		textures[imgLoc] = std::move(texture);
	}
	return textures[imgLoc];
}

bool Entity::Contains(const sf::Vector2f point) const
{
	sf::Vector2f topLeft = GetPos() - sprite.getOrigin();
	sf::Vector2f botRight = topLeft + sf::Vector2f((float)collisionBox.width, (float)collisionBox.height);
	if(botRight.x >= point.x && topLeft.x < point.x &&
		botRight.y >= point.y && topLeft.y < point.y) {
			//printf("Contains (%g,%g)\n", point.x, point.y);
			return true;
	}
	//printf("Doesn't contain (%g,%g)\n", point.x, point.y);
	return false;
}

bool Entity::IsColliding(const sf::IntRect& box) const
{
	//Separating Axis Theorem: Two boxes are colliding if and only if they are colliding
	// on their component axes (x and y)
	sf::IntRect thisCollisionBox = collisionBox;
	thisCollisionBox.left += (int)(GetPos().x - sprite.getOrigin().x);
	thisCollisionBox.top += (int)(GetPos().y - sprite.getOrigin().y);

	//Left side of the other box in this one
	bool leftCollide =
		(box.left >= thisCollisionBox.left &&
		box.left < thisCollisionBox.left + thisCollisionBox.width) || 
		(thisCollisionBox.left >= box.left &&
		thisCollisionBox.left < box.left + box.width);
	//Right side of other box in this one
	bool rightCollide = 
		(box.left + box.width > thisCollisionBox.left &&
		box.left + box.width < thisCollisionBox.left + thisCollisionBox.width) ||
		(thisCollisionBox.left + thisCollisionBox.width > box.left &&
		thisCollisionBox.left + thisCollisionBox.width < box.left + box.width);
	//Top side of the other box in this one
	bool topCollide = 
		(box.top >= thisCollisionBox.top &&
		box.top < thisCollisionBox.top + thisCollisionBox.height) ||
		(thisCollisionBox.top >= box.top &&
		thisCollisionBox.top < box.top + box.height);
	//Bottom side of the other box in this one
	bool botCollide = 
		(box.top + box.height > thisCollisionBox.top &&
		box.top + box.height < thisCollisionBox.top + thisCollisionBox.height) ||
		(thisCollisionBox.top + thisCollisionBox.height > box.top &&
		thisCollisionBox.top + thisCollisionBox.height < box.top + box.height);

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

sf::Vector2f Entity::lineIntersect(sf::Vector2f p1, sf::Vector2f p2, float epsilon) const {
	sf::Vector2f box[4];
	box[0] = sf::Vector2f(sprite.getPosition().x - sprite.getOrigin().x,
			sprite.getPosition().y - sprite.getOrigin().y);
	box[1] = box[0] + sf::Vector2f(collisionBox.width, 0.0f);
	box[2] = box[0] + sf::Vector2f(0.0f, collisionBox.height);
	box[3] = box[0] + sf::Vector2f(collisionBox.width, collisionBox.height);

	sf::Vector2f closest(FLT_MAX, FLT_MAX);
	float closestMag = FLT_MAX;

	for(unsigned i = 0; i < 4; i++) {
		unsigned j = (i+1)%4;
		sf::Vector2f intersect = ::lineIntersect(p1, p2, box[i], box[j]);
		float intersectMag = magnitude(p1 - intersect);
		if(intersectMag < closestMag && onLine(box[i], box[j], intersect, epsilon)) {
			closestMag = intersectMag;
			closest = intersect;
		}
	}
	
	return closest;
}

bool Entity::IsCollidable()
{
	return isCollidable;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if(visible)
		target.draw(sprite, state);

#if DRAW_COLBOXES == 1 
	sf::IntRect thisCollisionBox = collisionBox;
	thisCollisionBox.left += (int)GetPos().x - sprite.getOrigin().x;
	thisCollisionBox.top += (int)GetPos().y - sprite.getOrigin().y;
	sf::RectangleShape rect(sf::Vector2f(thisCollisionBox.width, thisCollisionBox.height));
	rect.setPosition(thisCollisionBox.left, thisCollisionBox.top);
	rect.setOutlineColor(sf::Color(0, 255, 0, 255));
	rect.setFillColor(sf::Color(0,0,0,0));
	rect.setOutlineThickness(1.0f);
	target.draw(rect, state);
#endif
}


void Entity::SetPos(sf::Vector2f newpos)
{
	sprite.setPosition(newpos);
}

sf::Vector2f Entity::GetPos() const
{
	return sprite.getPosition();
}

//See the comment for this function in the header file, for an explanation
// of why this function is needed.
void Entity::SetRotCorrected(float newrot) {
	sf::Vector2f origin(sprite.getOrigin().x, -sprite.getOrigin().y);
	float r = sqrt(origin.x * origin.x + origin.y * origin.y);
	float theta = atan2f(origin.y, origin.x) + newrot * TO_RAD;
	sf::Vector2f newOrigin(r * cos(theta), r * sin(theta));
	sf::Vector2f movement(newOrigin - origin);

	sprite.move(movement.x, movement.y);
	SetRot(newrot);
}

void Entity::SetRot(float newrot)
{
	sprite.setRotation(newrot);
	//printf("Got rot of %g\n", newrot);
}

void Entity::PlayAnim(const std::string& anim, const sf::Time& timePassed)
{
	animManager.PlayAnim(anim, timePassed);
	sprite.setTextureRect(animManager.GetCurAnimRect());
}
