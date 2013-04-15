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
#include "SoundManager.h"

#define DRAW_COLBOXES 1 

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
	Entity(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox, bool doesCollide);

	virtual void Update(Level& level, const sf::Time& timePassed) = 0;

	/**
	 * Checks if this entity's bounding box contains a certain point.
	 * \return True if the entity contains point, false otherwise.
	 */
	bool Contains(const sf::Vector2f point) const;

	/**
	 * Sets the Entity's position.
	 * \param newpos The new position of the Entity.
	 */
	void SetPos(sf::Vector2f newpos);

	/**
	 * Gets the Entity's position.
	 * \return The position of the entity.
	 */
	sf::Vector2f GetPos() const;

	/**
	 * Sets the Entity's rotation.
	 * \param newrot The new rotation of the Entity, in degrees.
	 */
	void SetRot(float newrot);

	/**
	 * Does a collision check on this entity and another.
	 * \param entity The entity to check collisions against.
	 * \return True if this entity is colliding with the other, false
	 *  otherwise.
	 */
	virtual bool IsColliding(const Entity& entity) const;

	/**
	 * Does a collision check on this entity and a bounding box.
	 * \param box The box to check collisions against.
	 * \return True if this entity is colliding with the other, false otherwise.
	 */
	virtual bool IsColliding(const sf::IntRect& box) const;

	/**
	 * Does a collision check on this entity and an attack cone.
	 * \param cone The cone to check collisions against.
	 * \return True if this entity is colliding with the cone, false otherwise.
	 */
	virtual bool IsColliding(const AttackCone& cone) const;

	/**
	 * Checks if this entity is collidable with the player or not.
	 * \return True if the entity should collide with the player, false otherwise.
	 */
	bool IsCollidable();
protected:
	/** Collision rectangle. */
	sf::IntRect collisionBox;
	/** Sprite to draw the texture on screen. */
	sf::Sprite sprite;
	/** Texture representing this entity. */
	sf::Texture texture;

	/** Flag set to true when the entity should be drawn to screen. */
	bool visible;

	/** Flag set to true when the entity should block the player. */
	bool isCollidable;

	/** Animation Manager. */
	AnimManager animManager;
	/** Sound manager. */
	SoundManager soundManager;

	/**
	 * Plays an animation.
	 *
	 * Needed because AnimManager::PlayAnim can't set the sprite's subrectangle.
	 * \param anim The name of the animation to play.
	 * \param timePassed The amount of time passed since the last call to this function.
	 */
	void PlayAnim(const std::string& anim, const sf::Time& timePassed);

	/**
	 * Draw the entity. Just calls the private draw().
	 *
	 * Here in case any derived class needs to draw something other than its sprite.
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
private:
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;
};

#endif
