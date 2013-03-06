/**
 * \brief An enemy that attacks the player.
 * \author Ed Lu
 * \Date March 2 2013
 */

#ifndef __LIGHTOVERLAY_H
#define __LIGHTOVERLAY_H

#include <SFML/Graphics.hpp>

#include <sstream>

class Level;

class LightOverlay : public sf::Drawable
{
public:
	/**
	 * Init constructor.
	 * \param rays The number of rays to cast when forming the overlay.
	 * \param radius The absolute cutoff distance for the light.
	 */
	LightOverlay(int rays, int radius, const sf::Vector2f& pos, const sf::View& view);

	void SetPos(const sf::Vector2f& pos);

	/**
	 * Updates the overlay, recomputing the lighting textures and repositioning
	 *  the overlay to the view.
	 * \param level The current level.
	 * \param view The active view.
	 */
	void Update(const Level& level, const sf::View& view);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** The image the circle gradient will be drawn to. */
	sf::Image circleImage;
	/** The texture that loads image. */
	sf::Texture circleTexture;
	/** The sprite that loads texture, which is drawn to the screen */
	sf::Sprite circleSprite;

	sf::RenderTexture triangleOverlay;
	sf::Sprite triangleOverlaySprite;

	/** Number of rays to use when updating the overlay. */
	int rays;
	/** Cutoff distance for the overlay. */
	int radius;
	/** Position of the light source */
	sf::Vector2f lightPos;
	/** Flag set to true when update needs to be run. */
	bool needsUpdate;

	std::vector<sf::CircleShape> circles;
	std::vector<sf::Text> texts;
	sf::Font font;
};

#endif