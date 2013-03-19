/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __LIGHTOVERLAY_H
#define __LIGHTOVERLAY_H

#include <SFML/Graphics.hpp>

#include <sstream>

#include "Utility.h"


class Level;

class LightSource : public sf::Drawable
{
public:
	/**
	 * Init constructor.
	 * \param rays The number of rays to cast when forming the overlay.
	 * \param radius The absolute cutoff distance for the light.
	 */
	LightSource(int rays, int radius, const sf::Color& color, const sf::Vector2f& pos);

	/**
	 * Sets the center position of the light. Also marks it for update.
	 * \param pos The light's new position.
	 */
	void SetPos(const sf::Vector2f& pos);

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
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** The color/intensity overlay circle. */
	sf::CircleShape circleOverlay;

	/** The image of the circle gradient. */
	sf::Image circleImage;
	/** The texture that holds circleImage. */
	sf::Texture circleTexture;
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

	/** Number of rays to use when updating the overlay. */
	int rays;
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
};

#endif