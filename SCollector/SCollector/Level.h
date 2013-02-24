/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Represents a level of the game.
 */

#ifndef __LEVEL_H
#define __LEVEL_H

#include <SFML/Graphics.hpp>

#include <cstdio>
#include <cmath>

#include "Tmx.h"

#define BASEMAPDIR "assets/maps/"

class Level : public sf::Drawable
{
public:
	/** Default constructor */
	Level();
	/**
	 * Init constructor
	 * \param mapName The name of the map in BASEMAPDIR to parse.
	 */
	Level(std::string mapName);

	/**
	 * Parses a .tmx format map.
	 * \param mapName The name of the map in BASEMAPDIR to parse.
	 * \return True if the map was parsed without error, false otherwise.
	 */
	bool Parse(std::string mapName);

	/**
	 * Gets the nearest colliding pixel in a certain diretcion.
	 *
	 * Takes a line and a direction and returns the nearest pixel in map coordinates
	 *  that is marked as colliding. If this function returns false, the value of
	 *  nearest should be considered invalid.
	 * \param pos The starting position of the line.
	 * \param horiz True if the line is horizontal, false if vertical.
	 * \param stepPos True if the line goes in the positive direction, false for negative.
	 * \param nearest The coordinates of the nearest colliding pixel.
	 * \return True if there is a collision in the line's direction, false otherwise.
	 */
	bool GetCollide(const sf::Vector2f& pos, const bool horiz, const bool stepPos, int& nearest) const;

	/**
	 * Gets the nearest non-colliding pixel in the negative y direction.
	 *
	 * Does nearly the same thing as GetCollide, but instead of finding a colliding pixel,
	 *  it finds a non-colliding pixel, always in the negative y direction. Note that nearest
	 *  is only valid if this function returns true.
	 * \param pos The position to start from.
	 * \param nearest The nearest non-colliding pixel in the negative y direction.
	 * \return True if there was any non-colliding pixel; false otherwise.
	 */
	bool AdjustY(const sf::Vector2f& pos, int& nearest) const;

	/**
	 * Gets a tile coordinate local to a tileset from a global layer tile.
	 * 
	 * \param layer The layer where globTile is located.
	 * \param globTile The global coordinate of the tile.
	 * \return The local coordinate of the tile, or sf::Vector2i(-1, -1) if an invalid tile was passed.
	 */
	sf::Vector2i GetLocalTile(const Tmx::Layer* layer, const sf::Vector2i& globTile) const;

	/**
	 * Gets the Tmx::tileset* of a global tile.
	 * 
	 * \param layer The layer where globTile is located.
	 * \param globTile The global coordinate of the tile.
	 * \return The tileset of the tile, or NULL if an invalid tile was passed.
	 */
	const Tmx::Tileset* GetTileset(const Tmx::Layer* layer, const sf::Vector2i& globTile) const;

	/**
	 * Gets the global tile number from a position on the map.
	 *
	 * \param pos The position to translate to a tile.
	 * \return The tile corresponding to that position.
	 */
	sf::Vector2i GetGlobalTile(const sf::Vector2f& pos) const;

	/**
	 * Gets the number of pixels within the tile that the position is.
	 *
	 * \param pos The position to translate to a tile.
	 * \return The pixel within the tile of the position.
	 */
	sf::Vector2i Level::GetPixel(const sf::Vector2f& pos) const;

	/**
	 * Gets the position of the spawn point.
	 * \return The position of the spawn point.
	 */
	sf::Vector2f GetSpawn() const;
private:
	/** Draws the level */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** Map of pixels for a collision tilemap */
	std::vector< std::vector<bool> > collisionMap;
	/** Tileset textures */
	std::vector<sf::Texture> texTilesets;
	/** Tileset sprites */
	std::vector<sf::Sprite> sprTilesets;
	/** TmxParser representation of the map */
	Tmx::Map map;

	/** Collision tileset */
	const Tmx::Tileset *tsetCollision;
	/** Collision layer */
	const Tmx::Layer *lyrCollision;

	/** Spawn point for the player */
	sf::Vector2f spawn;
};

#endif
