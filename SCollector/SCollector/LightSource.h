/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __LIGHTOVERLAY_H
#define __LIGHTOVERLAY_H

//Turn on for some neat diagnostics
#define DEBUGDRAW 0

#include <SFML/Graphics.hpp>

#include <sstream>

#include "Utility.h"

class Level;

class LightSource : public sf::Drawable
{
public:
	/**
	 * Init constructor.
	 * \param radius The absolute cutoff distance for the light.
	 */
	LightSource(int radius, const sf::Color& color, const sf::Vector2f& pos);

	/**
	 * Sets the center position of the light. Also marks it for update.
	 * \param pos The light's new position.
	 */
	void SetPos(const sf::Vector2f& pos);

	/**
	 * Forces this LightSource to update on the next call to Update()
	 */
	void ForceUpdate();

	/**
	 * Updates the overlay, recomputing the lighting textures and repositioning
	 *  the overlay to the view.
	 * \param level The current level.
	 * \param view The active view.
	 */
	void Update(const Level& level);

	/**
	 * Changes the color of the light.
	 * \param color The light's new color.
	 */
	void SetColor(const sf::Color& color);

	/**
	 * If the light is on, this sets it to off, and vice versa.
	 */
	void Toggle();

	/**
	 * Gets the state of the light.
	 * \return True if the light is on, false otherwise.
	 */
	bool GetIsOn();

	void debugDraw(sf::RenderTarget& target, sf::RenderStates state) const;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** Shared init function between constructors */
	void init(int radius, const sf::Color &color, const sf::Vector2f &pos);

	/** The color/intensity overlay circle. */
	sf::CircleShape circleOverlay;
	/** The sprite that loads circleTexture, which is drawn to the screen */
	sf::Sprite circleSprite;

	/** The visibility portion of the lighting, consisting of triangles */
	sf::RenderTexture triangleOverlay;
	/** The sprite associated with triangleOverlay */
	sf::Sprite triangleOverlaySprite;

	/** The lighted area, composited from circleSprite and triangleOverlaySprite */
	sf::RenderTexture compositeOverlay;
	/** The sprite associated with compositeOverlay */
	sf::Sprite compositeOverlaySprite;

#ifdef DEBUGDRAW
	std::vector<sf::CircleShape> circles;
	std::vector<sf::Text> texts;
	static sf::Font font;
	static bool fontLoaded;
#endif

	/** Cutoff distance for the overlay. */
	int radius;

	/** Position of the light source */
	sf::Vector2f lightPos;
	/** Flag set to true when update needs to be run. */
	bool needsUpdate;
	/** Flag set to true when the texture needs to be re-composited. */
	bool needsRedraw;
	/** Flag set to true when this light source is on. Defaults to false.*/
	bool isOn;

	/** Static texture for the circle shared by all light sources */
	static sf::Texture circleTexture;

};

#endif
