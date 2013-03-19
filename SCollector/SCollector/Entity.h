/**
 * \brief Represents an in-game entity that can be displayed.
 * \author Edward Lu
 * \date February 24 2013
 *
 * This is a subclass of sf::Drawable so that it can be drawn by itself.
 */

#ifndef __ENTITY_H
#define __ENTITY_H

#include <map>

#include <SFML/Graphics.hpp>

class Level;

class Entity : public sf::Drawable
{
public:
	/**
	 * Initializing Constructor.
	 * \param imgLoc The location of the image on the harddrive to load into texture.
	 * \param collisionBox The size of the entity's collision box.
	 * \param animBox The size of the frames in the sprite sheet, if any.
	 */
	Entity(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox);

	virtual void Update(const Level& level) = 0;

	/**
	 * Sets the Elayer's position.
	 * \param newpos The new position of the Entity.
	 */
	void SetPos(sf::Vector2f newpos);

	/**
	 * Gets the Entity's position.
	 * \return The position of the entity.
	 */
	sf::Vector2f GetPos() const;

	/**
	 * Does a collision check on this entity and another.
	 * \param entity The entity to check collisions against.
	 * \return True if this entity is colliding with the other, false
	 *  otherwise.
	 */
	bool IsColliding(const Entity& entity) const;

	/**
	 * Does a collision check on this entity and a bounding box.
	 * \param box The box to check collisions against.
	 * \return True if this entity is colliding with the other, false otherwise.
	 */
	bool Entity::IsColliding(const sf::IntRect& box) const;
protected:
	sf::IntRect collisionBox;
	sf::Sprite sprite;
	sf::Texture texture;

	bool visible;

	/**
	 * Adds a set of frames to the animation sets of this entity.
	 * begin and end denote the number of frames to begin and end at;
	 * if the image wraps around, simply number them like so:
	 *
	 * 0 1 2 3
	 * 4 5 6 7
	 *
	 * NOTE: The animation set "idle" is added with begin==0, end==0
	 *  on the creation of an entity.
	 * \param animName The name of this animation.
	 * \param begin The first frame of the animation.
	 * \param end The last frame of the animation.
	 * \param loop Flag that makes the animation loop when set to true.
	 * \return True if the animation was successfully added, false otherwise.
	 */
	bool AddAnimSet(const std::string& animName, int begin, int end, bool loop = false);

	/**
	 * Changes the properties of an existing animation.
	 * If begin or end are set to <0, then they remain unchanged.
	 *
	 * \param animName The name of this animation.
	 * \param begin The first frame of the animation.
	 * \param end The last frame of the animation.
	 * \param loop Flag that makes the animation loop when set to true.
	 * \return True if the animation was successfully modified, false otherwise.
	 */
	bool ModifyAnimSet(const std::string& animName, int begin, int end, bool loop = false);

	/**
	 * Plays an animation.
	 * If the animation is not currently the one that's playing, then
	 * StartAnim(anim) is called; otherwise, StepAnim() is called.
	 * \param anim The name of the animation to play.
	 */
	void PlayAnim(const std::string& anim);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/**
	 * Starts the entity playing the animation, if it exists.
	 * The animation set should have been added using AddAnimSet first.
	 * \param animName The name of the animation to start playing.
	 * \return True if the animation existed, false otherwise.
	 */
	bool StartAnim(const std::string& animName);

	/**
	 * Steps the current animation forward one frame.
	 * \return False if there is no current animation OR the current
	 *  animation finished; false otherwise.
	 */
	bool StepAnim();
	
	sf::IntRect GetCurAnimRect();

	sf::IntRect animBox;
	sf::IntRect sheetSize;
	std::map<std::string, int> animNames;
	std::vector<int> animSetBegin;
	std::vector<int> animSetEnd;
	std::vector<bool> animLoop;
	int curAnim;
	int curAnimFrame;
	int lastAnim;
	int totalFrames;

	sf::Clock lastAnimTime;
	const sf::Time animTime;
};

#endif
