/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Represents a level of the game.
 */

#ifndef __LEVEL_H
#define __LEVEL_H

#include <SFML/Graphics.hpp>

#include <climits>
#include <cstdio>
#include <cmath>
#include <cfloat>

#include <vector>
#include <list>

#include <TmxParser/Tmx.h>

#include "Utility.h"
#include "AttackCone.h"
class Player;
class Entity;
class Activatable;
class Movable;
class LightSource;
class HUD;

#define BASEMAPDIR "assets/maps/"

struct Corner {
	sf::Vector2f pos;
	int quadrant;
	bool inner;
	enum FacingType {
		FACING_AWAY,
		FACING_CORNER,
		FACING_TANGENT_FIRST,
		FACING_TANGENT_SECOND
	};
	FacingType getFacingType(const sf::Vector2f &pos) const;
};
struct Edge {
	std::size_t c1;
	std::size_t c2;
	sf::Vector2i normal;
};

//Note: Level is sort of an Entity too, but it's got too many
// differences to make it a subclass of Entity
class Level : public sf::Drawable
{
public:
	/**
	 * Init constructor
	 *
	 * \param player The player of the game.
	 */
	Level(Player& player);

	/**
	 * Init constructor
	 *
	 * Equivalent to calling Level() then LoadMap()
	 * \param mapName The name of the map in BASEMAPDIR to parse.
	 * \param spawnName Name of the spawn to look for within the map
	 * \param player The player of the game.
	 */
	Level(Player& player, const std::string& mapName, const std::string& spawnName = "any");

	/** Destructor. */
	~Level();

	/**
	 * Loads a map into the level object.
	 * \param mapName The name of the map in BASEMAPDIR to parse.
	 * \param spawnName the name of the spawn to look for within the map.
	 * \return True if the map was loaded successfully, false otherwise.
	 */
	bool LoadMap(const std::string& mapName, const std::string& spawnName = "any");

	/**
	 * Unloads the map currently loaded, and does cleanup.
	 *
	 * This should be called before loading another map, but not the first time
	 *  a map is loaded.
	 */
	void UnloadMap();

	/**
	 * Spawns the player at a new location without changing anything else about the map.
	 *
	 * This can be called if there was a level transition back to a level that was
	 *  already loaded.
	 * Note: If spawnName is not found in the map, this simply places the player at (0, 0).
	 * \param spawnName The spawn to put the player at.
	 */
	void SpawnPlayer(const std::string& spawnName = "any");
	void SetActive(bool active);

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
	 * \param timePassed The amount of time passed since the last time this function was called.
	 */
	void Update(const sf::Time& timePassed);

	/**
	 * Gets the player.
	 *
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
	bool GetCollide(const sf::Vector2f& pos, float angle, sf::Vector2f& nearest) const;
	bool GetCollide(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& point) const;

	/**
	 * Attempts to activate an object, if the player is colliding with one.
	 */
	void DoActivate();

	/**
	 * Adds a light to the level for drawing.
	 * \param light The light to be added.
	 */
	void AddLight(LightSource& light);

	/**
	 * Adds an attack to the level for processing.
	 * \param attack The attack to be added.
	 * \param enemy Flag set to true if this is an attack made by an enemy;
	 *  otherwise, it is assumed to be the player's attack.
	 */
	void AddAttack(const AttackCone& attack);

	/**
	 * Checks whether we are transitioning levels because the player activated
	 *  a level transition object.
	 * \param levelName Return parameter, valid only if the function returns true,
	 *  containing the name of the level to transition to.
	 * \param spawnName Return parameter, valid only if the function returns true,
	 *  containing the name of the spawnpoint in the next level.
	 * \return True if the level should transition, false otherwise.
	 */
	bool CheckLevelTransition(std::string& levelName, std::string& spawnName);

	/**
	 * Sets the next level to transition to.
	 * \param levelName Name of the level to transition to.
	 * \param spawnName Name of the spawn in the next level to spawn the player at.
	 */
	void SetLevelTransition(const std::string& levelName, const std::string& spawnName);

	/**
	 * Sets the text to be sent to the HUD on next call to UpdateHUD.
	 * \param text The text to display on the HUD.
	 */
	void SetHUDText(const std::string text);

	/**
	 * Updates the HUD with text and new values from the player.
	 * \param hud The HUD.
	 */
	void UpdateHUD(HUD& hud);

	bool onCorrectedEdge(Edge e, sf::Vector2f point, sf::Vector2f pos) const;

	struct LightPoint {
		sf::Vector2f fillTo;
		sf::Vector2f fillFrom;
	};

	std::vector<LightPoint> IntersectWalls(const sf::Vector2f &pos) const;
private:

	std::vector<Corner> corners;
	std::vector<Edge> edges;

	void GetEdges();

	/**
	 * Parses a .tmx format map.
	 * \param mapName The name of the map in BASEMAPDIR to parse.
	 * \param spawnName the name of the spawn we're looking for in the map.
	 * \return NULL if the map was not able to be parsed, a Tmx::Map* otherwise.
	 */
	Tmx::Map* Parse(const std::string& mapName);

	/**
	 * Initializes textures to be drawn to.
	 *
	 * Parse() must have been called beforehand and returned true.
	 */
	void InitTextures();

	/**
	 * Draws the map onto tilemapTexture.
	 *
	 * InitTextures() must have been called beforehand.
	 * \param map The map returned from Parse() to draw.
	 */
	void DrawMap();

	/**
	 * Helper function for GetCollide
	 */
	bool GetCollideHelp(const sf::Vector2f& pos, float tanDir, sf::Vector2f& nearest) const;

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
	sf::Vector2i GetPixel(const sf::Vector2f& pos) const;

	/**
	 * Gets a tile coordinate local to a tileset from a global layer tile.
	 * 
	 * \param layer The layer where globTile is located.
	 * \param globTile The global coordinate of the tile.
	 * \return The local coordinate of the tile, or sf::Vector2i(-1, -1) if an invalid tile was passed.
	 */
	sf::Vector2i GetLocalTile(const Tmx::Layer* layer, const sf::Vector2i& globTile) const;

	/**
	 * Gets a point in the collision map.
	 * \param locTile A local tile position from GetLocalTile.
	 * \param pixel The pixel, relative to locTile, to get.
	 * \return True if that pixel location should collide, false othwerise.
	 */
	bool GetColliding(const sf::Vector2i& locTile, const sf::Vector2i& pixel) const;

	/**
	 * Forces all lights in the level to update.
	 *
	 * Should be called when something in the level changes, such as a door opening.
	 */
	void ForceUpdateLights();

	/** Draws the level */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** Returns true if light passes through this tile */
	bool isLightPassable(sf::Vector2i globTile);

	/** Map of pixels for a collision tilemap */
	std::vector< std::vector<bool> > collisionMap;
	/** Tileset textures */
	std::vector<sf::Texture> texTilesets;
	/** Tileset sprites */
	std::vector<sf::Sprite> sprTilesets;

	/** Size of a single tile, in pixels. */
	sf::Vector2u tileSize;
	/** Size of the entire map, in tiles. */
	sf::Vector2u mapSize;

	/** Tmx-parser representation of the map; remove this later */
	Tmx::Map* map;

	/** Collision tileset */
	const Tmx::Tileset *tsetCollision;
	/** Collision layer */
	const Tmx::Layer *lyrCollision;

	/** Spawn point locations for the player*/
	std::map<std::string, sf::Vector2f> spawns;

	/** The tilemap texture */
	sf::RenderTexture tilemapTexture;
	/** The tilemap sprite */
	sf::Sprite tilemapSprite;

	/** The light overlay texture */
	sf::RenderTexture lightTexture;
	/** The light overlay sprite */
	sf::Sprite lightSprite;

	/**
	 * The player himself 
	 * He's just another entity, but it's worth keeping him around as a
	 *  separate entity for clarity sake
	 */
	Player& player;

	/** List(s) of entities in the level */
	std::vector<Activatable*> activatables;
	std::vector<Movable*> enemies;
	std::list<AttackCone> attackCones;

	/** A list of lights in the level */
	std::vector<LightSource*> lights;

	/** Whether or not this level is still active; if false, the level should transition */
	bool isActive;
	/** The next level and next spawn, if the level is not active */
	std::string nextLevel;
	std::string nextSpawn;
	/** The text to send to the HUD; if "", then don't send anything. */
	std::string HUDText;
	/** Description of the level. */
	std::string description;
};

#endif
