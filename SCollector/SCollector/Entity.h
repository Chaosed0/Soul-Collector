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

#include "AnimManager.h"

class Level;
class AttackCone;

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

	virtual void Update(const Level& level, const sf::Time& timePassed) = 0;

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
	bool IsColliding(const sf::IntRect& box) const;

	/**
	 * Does a collision check on this entity and an attack cone.
	 * \param cone The cone to check collisions against.
	 * \return True if this entity is colliding with the cone, false otherwise.
	 */
	bool IsColliding(const AttackCone& cone) const;
protected:
	/** Collision rectangle. */
	sf::IntRect collisionBox;
	/** Sprite to draw the texture on screen. */
	sf::Sprite sprite;
	/** Texture representing this entity. */
	sf::Texture texture;

	/** Flag set to true when the entity should be drawn to screen. */
	bool visible;

	/** Animation Manager. */
	AnimManager animManager;

	/**
	 * Plays an animation.
	 *
	 * Needed because AnimManager::PlayAnim can't set the sprite's subrectangle.
	 * \param anim The name of the animation to play.
	 * \param timePassed The amount of time passed since the last call to this function.
	 */
	void PlayAnim(const std::string& anim, const sf::Time& timePassed);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif
