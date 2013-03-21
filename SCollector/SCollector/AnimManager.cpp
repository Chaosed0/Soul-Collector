
#include "AnimManager.h"

const sf::Time AnimManager::defaultAnimDelay = sf::microseconds(50000);

AnimManager::AnimManager(const sf::IntRect& animBox)
{
	this->animBox = animBox;
	curAnim = 0;
	curAnimFrame = 0;
	lastAnimAdded = 0;

	animTimer = sf::Time::Zero;
	animDelay = defaultAnimDelay;

	//Create the default animation
	AddAnimSet("idle", 0, 0, true);
	//Play it to initialize the animation rectangle
	PlayAnim("idle", animDelay);
}

bool AnimManager::ModifyAnimSet(const std::string& animName, int begin, int end, bool loop)
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

void AnimManager::SetSheetSize(sf::Vector2u sheetSize)
{
	this->sheetSize = sheetSize;
	totalFrames = (sheetSize.x/animBox.width) * (sheetSize.y/animBox.height);
}

bool AnimManager::AddAnimSet(const std::string& animName, int begin, int end, bool loop)
{
	//Make sure the animation doesn't exist already
	//If it doesn't, add it to the maps
	if(animNames.find(animName) == animNames.end()) {
		animNames[animName] = lastAnimAdded;
		animSetBegin.push_back(begin);
		animSetEnd.push_back(end);
		animLoop.push_back(loop);
		lastAnimAdded++;
		return true;
	}
	//If the animation name already exists, return false
	return false;
}

bool AnimManager::StartAnim(const std::string& animName)
{
	//If the animation name exists in the list of animations,
	// start playing it
	if(animNames.find(animName) != animNames.end()) {
		curAnim = animNames[animName];
		curAnimFrame = animSetBegin[curAnim];
		return true;
	}
	return false;
}

bool AnimManager::StepAnim()
{
	if(curAnimFrame == animSetEnd[curAnim] && !animLoop[curAnim]) {
		return false;
	}
	if(curAnimFrame == animSetEnd[curAnim] && animLoop[curAnim]) {
		curAnimFrame = animSetBegin[curAnim];
		return true;
		//printf("End, loop, reset to %d\n", curAnimFrame);
	}
	else {
		//printf("Frame %d\n", curAnimFrame);
		curAnimFrame = (curAnimFrame+1)%totalFrames;
		return true;
	}
}

void AnimManager::PlayAnim(const std::string& anim, const sf::Time& timePassed)
{
	animTimer += timePassed;
	if(animTimer > animDelay)
	{
		if(curAnim == animNames[anim])
			StepAnim();
		else
			StartAnim(anim);
		animTimer = sf::Time::Zero;
	}
}

sf::IntRect AnimManager::GetCurAnimRect()
{
	sf::IntRect curAnimFrameRect;
	curAnimFrameRect.left = (curAnimFrame*animBox.width)%sheetSize.x;
	curAnimFrameRect.top = (curAnimFrame*animBox.width)/sheetSize.x*animBox.height;
	curAnimFrameRect.width = animBox.width;
	curAnimFrameRect.height = animBox.height;

	//printf("curFrame: %d, left:%d, top:%d, width:%d, height:%d\n", curAnimFrame, curAnimFrameRect.left, curAnimFrameRect.top, curAnimFrameRect.width, curAnimFrameRect.height);

	if(curAnimFrameRect.top+curAnimFrameRect.height > sheetSize.y)
		return animBox;
	return curAnimFrameRect;
}
