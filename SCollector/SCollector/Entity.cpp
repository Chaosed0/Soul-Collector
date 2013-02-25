
#include "Entity.h"
#include "Level.h"

Entity::Entity(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: animTime(sf::microseconds(50000))
{
	//Set default values
	this->collisionBox = collisionBox;
	this->animBox = animBox;
	curAnim = 0;
	curAnimFrame = 0;
	lastAnim = 0;
	visible = true;
	active = false;
	expended = false;

	//Load the texture
	texture.loadFromFile(imgLoc);
	sprite.setTexture(texture);
	sprite.setOrigin(animBox.width/2, animBox.height/2);

	totalFrames = (texture.getSize().x/animBox.width) * (texture.getSize().y/animBox.height);

	//Create the default animation and play it
	AddAnimSet("idle", 0, 0, true);
	PlayAnim("idle");
	sprite.setTextureRect(animBox);
}

bool Entity::ModifyAnimSet(const std::string& animName, int begin, int end, bool loop)
{
	//Make sure the animation exists already
	if(animNames.find(animName) != animNames.end()) {
		int anim = animNames[animName];
		if(begin >= 0)
			animSetBegin[anim] = begin;
		if(end >= 0)
			animSetEnd[anim] = end;
		animLoop[anim] = loop;
		return true;
	}
	//If the animation name does not exist, do nothing
	return false;
}

bool Entity::AddAnimSet(const std::string& animName, int begin, int end, bool loop)
{
	//Make sure the animation doesn't exist already
	//If it doesn't, add it to the maps
	if(animNames.find(animName) == animNames.end()) {
		animNames[animName] = lastAnim;
		animSetBegin.push_back(begin);
		animSetEnd.push_back(end);
		animLoop.push_back(loop);
		lastAnim++;
		return true;
	}
	//If the animation name already exists, return false
	return false;
}

bool Entity::StartAnim(const std::string& animName)
{
	//If the animation name exists in the list of animations,
	// start playing it
	if(animNames.find(animName) != animNames.end()) {
		curAnim = animNames[animName];
		curAnimFrame = animSetBegin[curAnim];
		sprite.setTextureRect(GetCurAnimRect());
		return true;
	}
	return false;
}

bool Entity::StepAnim()
{
	if(curAnimFrame == animSetEnd[curAnim] && !animLoop[curAnim]) {
		return false;
	}
	if(curAnimFrame == animSetEnd[curAnim] && animLoop[curAnim]) {
		sprite.setTextureRect(GetCurAnimRect());
		curAnimFrame = animSetBegin[curAnim];
		//printf("End, loop, reset to %d\n", curAnimFrame);
	}
	else {
		//printf("Frame %d\n", curAnimFrame);
		sprite.setTextureRect(GetCurAnimRect());
		curAnimFrame = (curAnimFrame+1)%totalFrames;
	}
}

void Entity::PlayAnim(const std::string& anim)
{
	if(lastAnimTime.getElapsedTime() >= animTime)
	{
		if(curAnim == animNames[anim])
			StepAnim();
		else
			StartAnim(anim);
		lastAnimTime.restart();
	}
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if(visible)
		target.draw(sprite, state);
}

sf::IntRect Entity::GetCurAnimRect()
{
	int width = texture.getSize().x;
	sf::IntRect curAnimFrameRect;
	curAnimFrameRect.left = (curAnimFrame*animBox.width)%width;
	curAnimFrameRect.top = (curAnimFrame*animBox.width)/width*animBox.height;
	curAnimFrameRect.width = animBox.width;
	curAnimFrameRect.height = animBox.height;

	//printf("curFrame: %d, left:%d, top:%d, width:%d, height:%d\n", curAnimFrame, curAnimFrameRect.left, curAnimFrameRect.top, curAnimFrameRect.width, curAnimFrameRect.height);

	if(curAnimFrameRect.top+curAnimFrameRect.height > texture.getSize().y)
		return animBox;
	return curAnimFrameRect;
}

void Entity::SetPos(sf::Vector2f newpos)
{
	sprite.setPosition(newpos);
}

sf::Vector2f Entity::GetPos()
{
	return sprite.getPosition();
}