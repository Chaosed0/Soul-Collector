/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Represents a level of the game.
 */

#ifndef __LEVEL_H
#define __LEVEL_H

#include <SFML/Graphics.hpp>

#include <limits.h>

#include <cstdio>
#include <cmath>

#include "Tmx.h"

#include "Player.h"
#include "Utility.h"
class Entity;

#define BASEMAPDIR "assets/maps/"

//Note: Level is sort of an Entity too, but it's got too many
// differences to make it a subclass of Entity
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
	 * Gets the size of the map.
	 * \return The size of the map, in pixels.
	 */
	sf::Vector2i GetSize() const;

	/**
	 * Updates the level
	 * This includes all entities in the level, such as
	 * - Monsters
	 * - Display of walls and such
	 */
	void Update();

	/**
	 * Gets the player.
	 * This is kind of an ugly way to do things, but it's
	 * better than the alternatives (I believe)
	 * \return A reference to the player.
	 */
	Player& GetPlayer(); 

	/**
	 * Get a const reference to the player.
	 * \return A const reference to the player.
	 */
	const Player& GetPlayer() const;

	/**
	 * Gets the nearest colliding pixel in a certain diretcion.
	 *
	 * Takes an axis direction and returns the nearest pixel in map coordinates
	 *  that is marked as colliding. If this function returns false, the value of
	 *  nearest should be considered invalid.
	 * NOTE: Changed this function to only return a nearest intersecting point if
	 *  there's one in the adjacent map tile. Otherwise, it just returns false.
	 * \param pos The starting position of the line.
	 * \param horiz True if the line is horizontal, false if vertical.
	 * \param stepPos True if the line goes in the positive direction, false for negative.
	 * \param nearest The coordinates of the nearest colliding pixel.
	 * \return True if there is a collision in the line's direction, false otherwise.
	 */
	bool GetCollide(const sf::Vector2f& pos, const bool horiz, const bool stepPos, int& nearest) const;

	/**
	 * Gets the nearest colliding pixel in a certain diretcion.
	 *
	 * Takes a direction and returns the nearest pixel in map coordinates
	 *  that is marked as colliding. If this function returns false, the value of
	 *  nearest should be considered invalid.
	 * NOTE: This function, unlike its other overload, returns the nearest intersecting
	 *  point no matter where it is.
	 * \param pos The starting position of the line.
	 * \param angle The angle of the ray we're shooting, in radians
	 * \param nearest The coordinates of the nearest colliding pixel.
	 * \return True if there is a collision in the line's direction, false otherwise.
	 */
	bool Level::GetCollide(const sf::Vector2f& pos, float angle, sf::Vector2f& nearest) const;
private:
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
	 * This tile includes the bottom right of the tile instead of
	 *  the top left.
	 *
	 * \param pos The position to translate to a tile.
	 * \return The tile corresponding to that position.
	 */
	sf::Vector2i GetGlobalTileBR(const sf::Vector2f& pos) const;

	/**
	 * Gets the number of pixels within the tile that the position is.
	 *
	 * \param pos The position to translate to a tile.
	 * \return The pixel within the tile of the position.
	 */
	sf::Vector2i GetPixel(const sf::Vector2f& pos) const;

	/**
	 * Gets a tile coordinate local to a tileset from a global layer tile.
	 * 
	 * \param layer The layer where globTile is located.
	 * \param globTile The global coordinate of the tile.
	 * \return The local coordinate of the tile, or sf::Vector2i(-1, -1) if an invalid tile was passed.
	 */
	sf::Vector2i GetLocalTile(const Tmx::Layer* layer, const sf::Vector2i& globTile) const;

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

	/**
	 * The player himself 
	 * He's just another entity, but it's worth keeping him around as a
	 *  separate entity for clarity sake
	 */
	Player player;
	/** List of entities in the level */
	std::vector<Entity*> entityList;
};

#endif
