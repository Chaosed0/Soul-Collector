
#ifdef LINUX
#include <float.h>
#endif

#include <unordered_set>

#include "Level.h"
#include "Entity.h"
#include "Key.h"
#include "Torch.h"
#include "Demon.h"
#include "Activatable.h"
#include "Movable.h"
#include "Stairs.h"
#include "Door.h"
#include "Player.h"
#include "LightSource.h"
#include "HUD.h"
#include "Soul.h"
#include "SlowDemon.h"
#include "HealthPot.h"
#include "FuelCanister.h"

Level::Level(Player& player)
	: player(player)
{
	mapSize = sf::Vector2u(0, 0);
	tileSize = sf::Vector2u(0, 0);
	lyrCollision = NULL;
	tsetCollision = NULL;
	isActive = false;
	HUDText = "";
}

Level::Level(Player& player, const std::string& mapName, const std::string& spawnName)
	: player(player)
{
	mapSize = sf::Vector2u(0, 0);
	tileSize = sf::Vector2u(0, 0);
	lyrCollision = NULL;
	tsetCollision = NULL;
	isActive = false;

	LoadMap(mapName, spawnName);
}

bool Level::isLightPassable(sf::Vector2i globTile) {
	if(globTile.x < 0 || globTile.x >= mapSize.x ||
			globTile.y < 0 || globTile.y >= mapSize.y) {
		//Count out-of-bounds as a solid wall
		return false;
	}
	
	int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
	const Tmx::Tile* tile = tsetCollision->GetTile(tileId);
	return tileId <= 0 || (tile != NULL ? tile->GetProperties().HasProperty("Clear") : false);
}

bool Level::LoadMap(const std::string& mapName, const std::string& spawnName)
{
	if(Parse(mapName) == NULL) {
		return false;
	}
	InitTextures();
	DrawMap();

	//Create the light overlay texture and sprite
	lightTexture.clear();
	lightTexture.display();
	lightSprite.setTexture(lightTexture.getTexture());
	//Add the player's light
	player.AddLight(*this);

	GetEdges();

	//Spawn the player
	SpawnPlayer(spawnName);

	//The level is now active and can be used.
	isActive = true;
	
	return true;
}

void Level::GetEdges() {
	std::unordered_set<int> cornerLocSet;

	//Collect the corners of the map
	for(unsigned x = 0; x <= mapSize.x; x++) {
		for(unsigned y = 0; y <= mapSize.y; y++) {
			bool upLeft = !isLightPassable(sf::Vector2i(x-1, y-1));
			bool upRight = !isLightPassable(sf::Vector2i(x, y-1));
			bool downLeft = !isLightPassable(sf::Vector2i(x-1, y));
			bool downRight = !isLightPassable(sf::Vector2i(x, y));
			int numSurroundingTiles = upLeft + upRight + downLeft + downRight;
			sf::Vector2i pos(x, y);
			switch(numSurroundingTiles) {
				case 1:
					{
						Corner corner;
						corner.pos = pos;
						corner.inner = false;
						if(upRight) {
							corner.quadrant = 4;
						} else if(upLeft) {
							corner.quadrant = 3;
						} else if(downLeft) {
							corner.quadrant = 2;
						} else if(downRight) {
							corner.quadrant = 1;
						}
						corners.push_back(corner);
						cornerLocSet.insert(pos.y * (mapSize.x+1) + pos.x);
						printf("%d %d %d\n", pos.x, pos.y, numSurroundingTiles);
					}
					break;
				case 2:
					{
						Corner corner1;
						Corner corner2;
						corner1.pos = pos;
						corner2.pos = pos;
						corner1.inner = true;
						corner2.inner = true;
						if(upLeft && downRight) {
							corner1.quadrant = 4;
							corner2.quadrant = 2;
							corners.push_back(corner1);
							corners.push_back(corner2);
							cornerLocSet.insert(pos.y * (mapSize.x+1) + pos.y);
							printf("%d %d %d\n", pos.x, pos.y, numSurroundingTiles);
						} else if(upRight && downLeft) {
							corner1.quadrant = 3;
							corner2.quadrant = 1;
							corners.push_back(corner1);
							corners.push_back(corner2);
							cornerLocSet.insert(pos.y * (mapSize.x+1) + pos.x);
							printf("%d %d %d\n", pos.x, pos.y, numSurroundingTiles);
						}
					}
					break;
				case 3:
					{
						Corner corner;
						corner.pos = pos;
						corner.inner = true;
						if(upLeft && downLeft && downRight) {
							corner.quadrant = 4;
						} else if(upRight && downLeft && downRight) {
							corner.quadrant = 3;
						} else if(upLeft && upRight && downRight) {
							corner.quadrant = 2;
						} else if(upLeft && upRight && downLeft) {
							corner.quadrant = 1;
						}
						corners.push_back(corner);
						cornerLocSet.insert(pos.y * (mapSize.x+1) + pos.x);
						printf("%d %d %d\n", pos.x, pos.y, numSurroundingTiles);
					}
					break;
				default:
					break;
			}
		}
	}

	//Pass through the corners of the map and determine edges
	for(std::size_t i = 0; i < corners.size(); i++) {
		if(corners[i].processed) { continue; }
		std::size_t current = i;
		Corner curCorner = corners[i];
		curCorner.processed = true;
		do {
			//Follow the edges in a clockwise fashion
			sf::Vector2i dir(0,0);
			sf::Vector2i normal(0,0);
			if((!curCorner.inner && curCorner.quadrant == 4) ||
					(curCorner.inner && curCorner.quadrant == 1)) {
				dir.x = 1;
				normal.y = 1;
			} else if((!curCorner.inner && curCorner.quadrant == 1) ||
					(curCorner.inner && curCorner.quadrant == 2)) {
				dir.y = 1;
				normal.x = -1;
			} else if((!curCorner.inner && curCorner.quadrant == 2) ||
					(curCorner.inner && curCorner.quadrant == 3)) {
				dir.x = -1;
				normal.y = -1;
			} else if((!curCorner.inner && curCorner.quadrant == 3) ||
					(curCorner.inner && curCorner.quadrant == 4)) {
				dir.y = -1;
				normal.x = 1;
			}

			sf::Vector2i pos;
			for(pos = curCorner.pos + dir;
					cornerLocSet.find(pos.y * (mapSize.x+1) + pos.x) == cornerLocSet.end();
					pos += dir) {
				if(pos.x < 0 || pos.x > (int)mapSize.x ||
						pos.y < 0 || pos.y > (int)mapSize.x) {
					fprintf(stderr, "WARNING: Encountered out-of-bounds while "
							"processing edges! Map is unbounded somewhere!\n");
					break;
				}
			}

			for(std::size_t j = 0; j < corners.size(); j++) {
				if(pos == corners[j].pos) {
					Edge edge;
					edge.c1 = current;
					edge.c2 = j;
					edge.normal = normal;
					edges.push_back(edge);
					corners[j].processed = true;
					curCorner = corners[j];
					current = j;
					break;
				}
				if(j == corners.size() - 1) {
					fprintf(stderr, "WARNING: Looks like an infinite loop processing "
							"corner position (%d, %d)\n", pos.x, pos.y);
					exit(1);
				}
			}
		} while(current != i);
	}
}

std::vector<Level::LightPoint> Level::IntersectWalls(const sf::Vector2f &pos) const {
	std::vector<LightPoint> points;
	for(std::size_t i = 0; i < corners.size(); i++) {
		bool tangent = false;

		Corner corner = corners[i];
		Corner::FacingType facing = corner.getFacingType(tileSize, pos);
		sf::Vector2f cornerPos(corner.pos.x * tileSize.x, corner.pos.y * tileSize.y);
		sf::Vector2f dir = cornerPos - pos;

		LightPoint point;
		sf::Vector2f closestIntersect(cornerPos);
		float closestMag = FLT_MAX;
		bool found = false;

		//The entire corner is facing away from us;
		//we don't need to process it
		if(facing == Corner::FACING_AWAY) {
			continue;
		}

		for(std::size_t j = 0; j < edges.size(); j++) {
			Edge edge = edges[j];
			Corner c1 = corners[edge.c1];
			Corner c2 = corners[edge.c2];
			sf::Vector2f c1pos(c1.pos.x * tileSize.x, c1.pos.y * tileSize.y);
			sf::Vector2f c2pos(c2.pos.x * tileSize.x, c2.pos.y * tileSize.y);
			sf::Vector2f edgeDir = c2pos - c1pos;

			//If this corner is a tangent corner and it's one of
			//the ones on this edge, skip it but note down that we
			//found it for later
			if(edge.c1 == i || edge.c2 == i) {
				tangent = (facing == Corner::FACING_TANGENT_FIRST ||
						facing == Corner::FACING_TANGENT_SECOND);
				if(tangent) { continue; }
			} else if((edge.normal.x != 0 && sign(dir.x) == edge.normal.x) ||
					(edge.normal.y != 0 && sign(dir.y) == edge.normal.y)) {
				//This edge is facing away from us; we don't need to process it
				continue;
			}

			sf::Vector2f intersect = lineIntersect(pos, cornerPos, c1pos, c2pos);
			float intersectMag = magnitude(pos - intersect);

			//If we intersected on the segment and this is a closer
			// intersection than any prior, take this as the new closest
			// intersection
			if(intersectMag < closestMag &&
					sign(intersect.x - pos.x) == sign(dir.x) &&
					sign(intersect.y - pos.y) == sign(dir.y) &&
					onCorrectedEdge(edge, intersect, pos)) {
				closestIntersect = intersect;
				closestMag = intersectMag;
				found = true;
			}
		}

		float distClosestCaster = magnitude(closestIntersect - pos);
		float distCornerCaster = magnitude(cornerPos - pos);
		float distClosestCorner = magnitude(closestIntersect - cornerPos);
		if(found && ((tangent && distClosestCaster - distCornerCaster > 1.0f)
				|| distClosestCorner <= 1.0f)) {
			point.fillTo = closestIntersect;
			point.fillFrom = closestIntersect;
			if(closestMag >= magnitude(dir)) { 
				if(tangent && facing == Corner::FACING_TANGENT_FIRST) {
					point.fillFrom = cornerPos;
				} else if(tangent && facing == Corner::FACING_TANGENT_SECOND) {
					point.fillTo = cornerPos;
				}
			}
			points.push_back(point);
		}
	}
	return points;
}

bool Level::onCorrectedEdge(Edge e, sf::Vector2f point, sf::Vector2f pos) const {
	Corner c1 = corners[e.c1];
	Corner c2 = corners[e.c2];
	sf::Vector2f c1pos(c1.pos.x * tileSize.x, c1.pos.y * tileSize.y);
	sf::Vector2f c2pos(c2.pos.x * tileSize.x, c2.pos.y * tileSize.y);
	float crossproduct = (point.y - c1pos.y) * (c2pos.x - c1pos.x) - (point.x - c1pos.x) * (c2pos.y - c1pos.y);
	/*bool c1tangent = c1.getFacingType(tileSize, pos) == Corner::FACING_TANGENT_FIRST ||
		c1.getFacingType(tileSize, pos) == Corner::FACING_TANGENT_SECOND;
	bool c2tangent = c2.getFacingType(tileSize, pos) == Corner::FACING_TANGENT_FIRST ||
		c2.getFacingType(tileSize, pos) == Corner::FACING_TANGENT_SECOND;*/
	float correction = 0.01f;

	return std::abs(crossproduct) < 0.5f &&
		point.x >= std::min(c1pos.x, c2pos.x) - correction &&
		point.x <= std::max(c1pos.x, c2pos.x) + correction &&
		point.y >= std::min(c1pos.y, c2pos.y) - correction &&
		point.y <= std::max(c1pos.y, c2pos.y) + correction;
}

void Level::SpawnPlayer(const std::string& spawnName)
{
	std::map<std::string, sf::Vector2f>::iterator spawnIter;
	spawnIter = spawns.find(spawnName);
	if(spawnIter != spawns.end()) {
		player.SetPos(spawnIter->second);
	} else {
		printf("WARNING: Spawnpoint %s not found! Defaulting to (0, 0)...\n", spawnName.c_str());
		player.SetPos(sf::Vector2f(0, 0));
	}
}

void Level::SetActive(bool active)
{
	isActive = active;
	//If the level was reactivated, we want to display a message to the player
	if(active) {
		SetHUDText("Now in " + description + ".");
	}
}

void Level::UnloadMap()
{
	//Destroy all pointer vectors
	for(unsigned int i = 0; i < activatables.size(); i++)
		delete(activatables[i]);
	activatables.clear();
	for(unsigned int i = 0; i < enemies.size(); i++)
		delete(enemies[i]);
	enemies.clear();
	//We don't need to delete the lights; since they're all owned by some entity or another,
	// they'll get deleted there instead
	lights.clear();
	//Remove all attack cones
	attackCones.clear();
	//Clear the collision map
	collisionMap.clear();
	texTilesets.clear();
	sprTilesets.clear();
	//Don't delete the collision tileset/layer, just set them to null
	tsetCollision = NULL;
	lyrCollision = NULL;
}

Level::~Level()
{
	//Destroy all pointer vectors
	for(unsigned int i = 0; i < activatables.size(); i++)
		delete(activatables[i]);
	activatables.clear();
	for(unsigned int i = 0; i < enemies.size(); i++)
		delete(enemies[i]);
	//Delete the Tmx::Map
	delete map;
}

void Level::InitTextures()
{
	tilemapTexture.create(mapSize.x*tileSize.y, mapSize.y*tileSize.y);
	lightTexture.create(mapSize.x*tileSize.y, mapSize.y*tileSize.y);
}

void Level::DrawMap()
{
	//Draw to the tilemap texture
	for(int i = 0; i < map->GetNumLayers(); i++)
	{
		const Tmx::Layer *layer = map->GetLayer(i);

		//Render tiles of the layer
		for(int y = 0; y < layer->GetHeight(); y++)
		{
			for(int x = 0; x < layer->GetWidth(); x++)
			{
				//If there is no tile on this location in the map, GetTileTilesetIndex()
				// returns -1, so just do nothing
				int tilesetIdx = layer->GetTileTilesetIndex(x, y);
				//printf("tilesetIdx: %d\n", tilesetIdx);
				if(tilesetIdx >= 0)
				{
					//Otherwise, get the sprite representing that tileset
					//const Tmx::Tileset *tileset = map->GetTileset(tilesetIdx);

					//Map the tile index to a coordinate in the tileset
					//unsigned int tileIdx = layer->GetTileId(x, y);
					sf::Vector2i locTile = GetLocalTile(layer, sf::Vector2i(x, y));

					if(locTile.x >= 0 && locTile.y >= 0)
					{
						//Get the part of the sprite corresponding to the tile
						sprTilesets[tilesetIdx].setTextureRect(sf::IntRect(locTile.x, locTile.y, map->GetTileWidth(), map->GetTileHeight()));
						//Set the tile's position to the correct place
						sprTilesets[tilesetIdx].setPosition((float)x*map->GetTileWidth(), (float)y*map->GetTileHeight());

						//Draw that tile
						tilemapTexture.draw(sprTilesets[tilesetIdx]);
					}
				}
			}
		}
	}
	//We're finished drawing to it - let SFML finalize the texture
	tilemapTexture.display();
	tilemapSprite.setTexture(tilemapTexture.getTexture());
}

Tmx::Map* Level::Parse(const std::string& mapName)
{
	//Initialize the map pointer
	map = new Tmx::Map();

	printf("Parsing map...\n");
	//Attempt to parse the map, and report any errors
	map->ParseFile(BASEMAPDIR + mapName);

	//Initialize some properties
	mapSize = sf::Vector2u(map->GetWidth(), map->GetHeight());
	tileSize = sf::Vector2u(map->GetTileWidth(), map->GetTileHeight());
	
	if(map->HasError()) {
		fprintf(stderr, "WARNING: Could not parse level %s: %s\n",
			mapName.c_str(), map->GetErrorText().c_str());
		return NULL;
	}

	//Make the tilesets of the map into sprites
	texTilesets.resize(map->GetNumTilesets());
	sprTilesets.resize(map->GetNumTilesets());
	for(int i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);
		if(!texTilesets[i].loadFromFile(BASEMAPDIR + tileset->GetImage()->GetSource()))
			fprintf(stderr, "Error loading tileset %d: %s\n", i, tileset->GetName().c_str());
		sprTilesets[i] = sf::Sprite(texTilesets[i]);

		//If this is the collision tileset, we want to make a collision map out of it
		if(tileset->GetName().compare("Collision") == 0) {
			//Save the tileset
			tsetCollision = tileset;		

			sf::Image imgCollision;
			//Load the image as an sf::Image
			if(!imgCollision.loadFromFile(BASEMAPDIR + tileset->GetImage()->GetSource()))
				fprintf(stderr, "Error loading image %d: %s\n", i, tileset->GetName().c_str());
			//Iterate through and figure out which pixels are transparent and not
			sf::Vector2u size = imgCollision.getSize();
			collisionMap.resize(size.x);

			for(unsigned int x = 0; x < size.x; x++) {
				collisionMap[x].resize(size.y);
				for(unsigned int y = 0; y < size.y; y++) {
					//The player should be able to collide with the tile if the
					// tile is not completely transparent
					collisionMap[x][y] = (imgCollision.getPixel(x, y).a > 0);
				}
			}
			//printf("(%d, %d): %d, %d\n", 33, 0, imgCollision.getPixel(33, 0).a, (collisionMap[33][0]==true));
		}
	}

	//We also need to find the collision layer
	for(int i = 0; i < map->GetNumLayers(); i++) {
		const Tmx::Layer *layer = map->GetLayer(i);
		//If this is the collision layer, store it
		if(layer->GetName().compare("Collision") == 0) {
			lyrCollision = layer;		
		}
	}

	if(lyrCollision == NULL)
		fprintf(stderr, "WARNING: Could not find collision layer! Attempting to proceed...\n");
	if(tsetCollision == NULL)
		fprintf(stderr, "WARNING: Could not find collision tileset! Attempting to proceed...\n");

	//Now, parse the map objects
	for (int i = 0; i < map->GetNumObjectGroups(); ++i) {
		//Get an object group.
		const Tmx::ObjectGroup *objectGroup = map->GetObjectGroup(i);

		//Iterate through all objects in the object group
		for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
			//Grab objects from the group
			const Tmx::Object *object = objectGroup->GetObject(j);
			const std::string &type = object->GetType();
			sf::Vector2f objectPos((float)object->GetX() + object->GetWidth()/2.0f,
						(float)object->GetY() - object->GetHeight()/2.0f);

			//Check the objects against our known object types...
			//Is the object a (the) spawn?
			if(type.compare("Spawn") == 0) {
				//If the spawn has no name, we can't store it
				std::string spawnName = object->GetName();
				if(spawnName.empty()) {
					fprintf(stderr, "WARNING: Spawn at %d, %d has no name! Ignoring...\n",
						object->GetX(), object->GetY());
				} else {
					spawns.insert(std::pair<std::string, sf::Vector2f>(spawnName, objectPos));
				}
			}
			else if(type.compare("Key") == 0) {
				std::string door = object->GetProperties().GetLiteralProperty("Door");
				std::string description = object->GetProperties().GetLiteralProperty("Description");
				int r = object->GetProperties().GetNumericProperty("Red");
				int b = object->GetProperties().GetNumericProperty("Blue");
				int g = object->GetProperties().GetNumericProperty("Green");
				if(door.empty()) {
					fprintf(stderr, "WARNING: Key has no associated door! Ignored...");
				} else {
					activatables.push_back(new Key(objectPos, door, description, sf::Color(r,g,b,255)));
					activatables.back()->SetRot(object->GetRot());
				}
			}
			else if(type.compare("Torch") == 0) {
				activatables.push_back(new Torch(objectPos));
				//This is a bad thing and I feel bad for doing it
				((Torch*)activatables.back())->AddLight(*this);
				activatables.back()->SetRot(object->GetRot());
			} else if(type.compare("Demon") == 0) {
				enemies.push_back(new Demon(objectPos));
				enemies.back()->SetRot(object->GetRot());
			} else if(type.compare("SlowDemon") == 0) {
				enemies.push_back(new SlowDemon(objectPos));
				enemies.back()->SetRot(object->GetRot());
			} else if(type.compare("Stairs") == 0) {
				std::string nextLevel = object->GetProperties().GetLiteralProperty("NextLevel");
				std::string nextSpawn = object->GetProperties().GetLiteralProperty("NextSpawn");
				if(nextLevel.empty() || nextSpawn.empty()) {
					fprintf(stderr, "WARNING: Stairs named %s have missing properties! Ignored...\n",
						object->GetName().c_str());
				} else {
					activatables.push_back(new Stairs(objectPos, nextLevel, nextSpawn));
					activatables.back()->SetRot(object->GetRot());
				}
			} else if(type.compare("Door") == 0) {
				std::string description = object->GetProperties().GetLiteralProperty("Description");
				activatables.push_back(new Door(objectPos, object->GetName(), description));
				activatables.back()->SetRot(object->GetRot());
			} else if(type.compare("Soul") == 0) {
				activatables.push_back(new Soul(objectPos));
				activatables.back()->SetRot(object->GetRot());
			} else if(type.compare("Potion") == 0) {
				int recovery = object->GetProperties().GetNumericProperty("Recovery");
				activatables.push_back(new HealthPot(objectPos, recovery));
				activatables.back()->SetRot(object->GetRot());
			} else if(type.compare("Fuel") == 0) {
				int recovery = object->GetProperties().GetNumericProperty("Recovery");
				activatables.push_back(new FuelCanister(objectPos, recovery));
				activatables.back()->SetRot(object->GetRot());
			}
			//We don't know what type of object this is, issue an error
			else {
				fprintf(stderr, "WARNING: Found unknown object type %s\n", object->GetType().c_str());
			}
		}
	}

	//Get a map description to display to the player.
	description = map->GetProperties().GetLiteralProperty("Description");
	printf("Map parsing complete\n");
	return map;
}

const Tmx::Tileset* Level::GetTileset(const Tmx::Layer* layer, const sf::Vector2i& globTile) const
{
	int tilesetIdx = layer->GetTileTilesetIndex(globTile.x, globTile.y);
	return (tilesetIdx < 0 ? NULL : map->GetTileset(tilesetIdx));
}

sf::Vector2i Level::GetLocalTile(const Tmx::Layer* layer, const sf::Vector2i& globTile) const
{
	const Tmx::Tileset* tileset = GetTileset(layer, globTile);

	if(tileset == NULL) {
		return sf::Vector2i(-1,-1);
	}
	int tileWidth = map->GetTileWidth();
	int tileHeight = map->GetTileHeight();
	int tilesetWidth = tileset->GetImage()->GetWidth() / tileWidth;
	int tileIdx = layer->GetTileId(globTile.x, globTile.y);

	if(tileIdx != 0)
		return sf::Vector2i(tileIdx % tilesetWidth * tileWidth, tileIdx / tilesetWidth * tileHeight);
	else
		return sf::Vector2i(-1, -1);
}

sf::Vector2i Level::GetGlobalTile(const sf::Vector2f& pos) const
{
	return sf::Vector2i(std::lround(pos.x) / tileSize.x, std::lround(pos.y) / tileSize.y);
}

sf::Vector2i Level::GetPixel(const sf::Vector2f& pos) const
{
	return sf::Vector2i(std::lround(pos.x) % map->GetTileWidth(), std::lround(pos.y) % map->GetTileHeight());
}

bool Level::GetColliding(const sf::Vector2i& locTile, const sf::Vector2i& pixel) const
{
	return collisionMap[locTile.x + pixel.x][locTile.y + pixel.y];
}

/*bool Level::GetCollide(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& point) const
{
	bool steep = std::abs(p2.y - p1.y) > std::abs(p2.x - p1.x);

	if(steep) {
		std::swap(p1.x, p1.y);
		std::swap(p2.x, p2.y);
	}
	if(p1.x > p2.x) {
		std::swap(p1.x, p2.x);
		std::swap(p1.y, p2.y);
	}

	int deltax = p2.x - p1.x;
	int deltay = std::abs(p2.y - p1.y);
	int error = deltax / 2;
	int ystep;
	int y = p1.y;
	bool collided = false;

	if(p1.y < p2.y) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	sf::Vector2f prevPoint;
	sf::Vector2i globTile(0,0);
	int x = p1.x;
	int dir = sign(p2.x - p1.x);
	while(!collided && globTile.x >= 0 && globTile.x < mapSize.x &&
			globTile.y >= 0 && globTile.y < mapSize.y) {
		x += dir;
		prevPoint = point;
		point.x = (steep ? y : x);
		point.y = (steep ? x : y);

		globTile = GetGlobalTile(point);
		sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
		sf::Vector2i pixel((int)(point.x-globTile.x*tileSize.x),
			(int)(point.y-globTile.y*tileSize.y));
		int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		const Tmx::Tile* tile = tsetCollision->GetTile(tileId);
		bool tileColliding = false;
		bool foundObj = false;

		//List of entities to also check for collisions
		sf::IntRect rect((int)(globTile.x*tileSize.x), (int)(globTile.y*tileSize.y),
			(int)tileSize.x, (int)tileSize.y);
		for(unsigned i = 0; i < activatables.size() && !foundObj; i++) {
			foundObj = activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect);
		}

		tileColliding = tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true);

		//If the tile is identified as colliding with light or we have an entity
		// to look for
		if(tileColliding || foundObj) {
			//If the pixel of the tile is colliding, then we have found a colliding tile
			if(tileColliding) {
				collided = GetColliding(locTile, pixel);
			}
			for(unsigned i = 0; i < activatables.size() && !collided; i++) {
				collided = collided ||
					(activatables[i]->IsCollidable() && activatables[i]->Contains(point));
			}

			if(collided) {
				//Otherwise, note down where we found this and break
				//Back out; the previous point was the last non-colliding one
				point = prevPoint;
			}
		}

		error = error - deltay;
		if(error < 0) {
			y = y + ystep;
			error = error + deltax;
		}
	}
	return collided;
}*/

bool Level::GetCollide(const sf::Vector2f& pos, float angle, sf::Vector2f& nearest) const
{
	bool extendX = false, extendY = false;


	bool foundObj = false;
	angle = shiftAngle(angle);
	//printf("%g\n", angle);
	float tanDir = tan(angle);
	//printf("%g\n", tanDir);

	//The first iteration is different from the others because we
	// need to figure out how far away the first block is
	sf::Vector2f nearestX(pos);
	sf::Vector2i globTile = GetGlobalTile(nearestX);
	float distX = FLT_MAX, distY = FLT_MAX;
	bool foundX = false, foundY = false;
	bool negXDir = angle > PI/2.0F || angle < -PI/2.0f;
	bool negYDir = angle < 0;

	//printf("X DIRECTION\n\n");

	//Do the x side first; while we're within map bounds...
	while(nearestX.x >= 0 && nearestX.x < (int)mapSize.x &&
			nearestX.y >= 0 && nearestX.y < (int)mapSize.y &&
			!foundX) {
		int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		const Tmx::Tile* tile = tsetCollision->GetTile(tileId);

		//List of entities to also check for collisions
		foundObj = false;
		sf::IntRect rect((int)(globTile.x*tileSize.x), (int)(globTile.y*tileSize.y),
			(int)tileSize.x, (int)tileSize.y);
		for(unsigned int i = 0; i < activatables.size() && !foundObj; i++) {
			foundObj = activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect);
		}

		//If the tile is identified as colliding with light or we have an entity
		// to look for
		if((tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true))
				|| foundObj) {
			sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
			sf::Vector2i pixel((int)(nearestX.x-globTile.x*tileSize.x),
				(int)(nearestX.y-globTile.y*tileSize.y));
			//Check the pixels of the tile along this line
			while(pixel.x >= 0 && pixel.x < (int)tileSize.x &&
					pixel.y >= 0 && pixel.y < (int)tileSize.y &&
					!foundX) {

				//If the pixel of the tile is colliding, then we have found a colliding tile
				if(tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true)) {
					foundX = GetColliding(locTile, pixel);
				}
				for(unsigned int i = 0; i < activatables.size(); i++) {
					bool oldFoundX = foundX;
					foundX = foundX || (activatables[i]->IsCollidable() && activatables[i]->Contains(nearestX));
					//HACK: Doors are kind of hard to see, we're just going to extend the ray a bit
					if(!oldFoundX && foundX)
						extendX = true;
				}

				if(!foundX) {
					//If this pixel is not colliding, then keep searching
					if(negXDir) nearestX.x--; else nearestX.x++;
					if(negYDir) nearestX.y-=abs(tanDir); else nearestX.y+=abs(tanDir);
					pixel.x = (int)(nearestX.x-globTile.x*tileSize.x);
					pixel.y = (int)(nearestX.y-globTile.y*tileSize.y);
				} else {
					//Otherwise, note down where we found this and break
					//Back out one pixel; the previous pixel was the last non-colliding one
					if(negXDir) nearestX.x++; else nearestX.x--;
					if(negYDir) nearestX.y+=abs(tanDir); else nearestX.y-=abs(tanDir);
					sf::Vector2f relDist = pos - nearestX;
					distX = magnitude(relDist);
				}
			}
		}

		//If we keep going, find the next tile
		if(!foundX) {
			if(!negXDir) {
				globTile.x++;
				nearestX.x = globTile.x*tileSize.x;
			} else {
				globTile.x--;
				//GetGlobalTile says tiles include their top-left edges; we have to
				// fix that for lines going in the negative direction
				nearestX.x = (globTile.x+1)*tileSize.x-1;
			}
			nearestX.y = pos.y + (tanDir*(nearestX.x - pos.x));
			globTile = GetGlobalTile(nearestX);
			//printf("Nearest: (%g, %g)\n", nearest.x, nearest.y);
		}
	}

	//Do the exact same thing for the y side
	sf::Vector2f nearestY(pos);
	globTile = GetGlobalTile(pos);

	//printf("Y DIRECTION\n\n");

	while(nearestY.y >= 0 && nearestY.y < (int)mapSize.x &&
			nearestY.x >= 0 && nearestY.x < (int)mapSize.y &&
			!foundY) {
		int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		const Tmx::Tile* tile = tsetCollision->GetTile(tileId);

		//List of entities to also check for collisions
		foundObj = false;
		sf::IntRect rect((int)(globTile.x*tileSize.x), (int)(globTile.y*tileSize.y),
			(int)tileSize.x, (int)tileSize.y);
		for(unsigned int i = 0; i < activatables.size() && !foundObj; i++) {
			foundObj = activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect);
		}

		if((tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true))
				|| foundObj) {
			sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
			sf::Vector2i pixel((int)(nearestY.x-globTile.x*tileSize.x),
				(int)(nearestY.y-globTile.y*tileSize.y));
			//Check the pixels of the tile along this line
			while(pixel.x >= 0 && pixel.x < (int)tileSize.x &&
					pixel.y >= 0 && pixel.y < (int)tileSize.y &&
					!foundY) {

				//Check if this tile is colliding with the ray, if we even have a tile to check
				if(tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true)) {
					foundY = GetColliding(locTile, pixel);
				}
				for(unsigned int i = 0; i < activatables.size(); i++) {
					bool oldFoundY = foundY;
					foundY = foundY || (activatables[i]->IsCollidable() && activatables[i]->Contains(nearestY));
					if(!oldFoundY && foundY)
						extendY = true;
				}

				if(!foundY) {
					//If this pixel is not colliding, then keep searching
					float quot = 1/std::fabs(tanDir);
					if(negYDir) nearestY.y--; else nearestY.y++;
					if(negXDir) nearestY.x-=quot; else nearestY.x+=quot;
					pixel.x = (int)(nearestY.x-globTile.x*tileSize.x);
					pixel.y = (int)(nearestY.y-globTile.y*tileSize.y);
				} else {
					float quot = 1/std::fabs(tanDir);
					//Otherwise, note down where we found this and break
					if(negYDir) nearestY.y++; else nearestY.y--;
					if(negXDir) nearestY.x+=quot; else nearestY.x-=quot;
					sf::Vector2f relDist = pos - nearestY;
					distY = magnitude(relDist);
				}
			}
		}

		if(!foundY) {
			if(!negYDir) {
				globTile.y++;
				nearestY.y = globTile.y*tileSize.y;
			} else {
				globTile.y--;
				nearestY.y = (globTile.y+1)*tileSize.y-1;
			}
			nearestY.x = (nearestY.y - pos.y) / tanDir + pos.x;
			globTile = GetGlobalTile(nearestY);
			//printf("Nearest: (%g, %g)\n", nearest.x, nearest.y);
		}
	}

	if(extendX) {
		sf::Vector2f relDist = nearestX-pos;
		float mag = magnitude(relDist);
		relDist = relDist * (mag+8.0f)/mag;
		nearestX = pos+relDist;
	}
	if(extendY) {
		sf::Vector2f relDist = nearestY-pos;
		float mag = magnitude(relDist);
		relDist = relDist * (mag+8.0f)/mag;
		nearestY = pos+relDist;
	}

	if(distX < distY) {
		nearest = nearestX;
	} else {
		nearest = nearestY;
	}
	//printf("%g, %g, returned %g\n", distX, distY, dist);
	return foundX || foundY;
}

bool Level::GetCollide(const sf::Vector2f& pos, const bool horiz, const bool stepPos, int& nearest) const
{
	const int maxSteps = 2;

	//We need to figure out which tile on the collision tileset this
	// position corresponds to on the layer
	sf::Vector2i globTile = GetGlobalTile(pos);
	sf::Vector2i pixel = GetPixel(pos);

	int layerHeight = lyrCollision->GetHeight();
	int layerWidth = lyrCollision->GetWidth();
	int& pixelRef = (horiz?pixel.x:pixel.y);
	int& tileRef = (horiz?globTile.x:globTile.y);
	nearest = (stepPos?INT_MAX:-INT_MAX);
	//We haven't found any colliding tile yet
	bool foundCol = false;

	//Get the nearest tile in the layer corresponding to the position
	//BIG NOTE !!!!!!!!! -----------------------
	// We assume that tile 0 is unused because that's what GetTileId returns
	// when there's no tile. It also can be a valid tile id, though!

	//Identify whether we are going in a positive or negative direction
	int step;
	if(stepPos == true)
		step = 1;
	else
		step = -1;

	for(int i = 0; i < maxSteps && !foundCol; i++) {
		//Make sure we're within the map bounds
		if(globTile.y >= 0 && globTile.y < layerHeight &&
				globTile.x >= 0 && globTile.x < layerWidth)
		{
			int oldPixel = pixelRef;
			//Get the tile ID
			unsigned int tileIdx = lyrCollision->GetTileId(globTile.x, globTile.y);

			//If the tile ID is 0, this tile isn't a colliding tile
			if(tileIdx != 0)
			{
				//Do pixel-by-pixel checks until we reach a colliding pixel
				sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
				while(pixel.x < map->GetTileWidth() && pixel.x >= 0 &&
						pixel.y < map->GetTileHeight() && pixel.y >= 0 && 
						!foundCol)
				{
					//Once we find a colliding pixel, return its location
					if(GetColliding(locTile, pixel)) {
						int newNearest = tileRef * (horiz?map->GetTileWidth():map->GetTileHeight())
							+ pixelRef - step;
						if(abs(newNearest) < abs(nearest)) {
							nearest = newNearest;
							foundCol = true;
						}
					}
					pixelRef += step;				
				}
			}

			//Reset the pixel so we can check object collisions as well
			pixelRef = oldPixel;

			//Iterate through all the possible colliding objects and check if
			// they are on this tile
			sf::IntRect rect(globTile.x*(int)tileSize.x, globTile.y*(int)tileSize.y,
				(int)tileSize.x, (int)tileSize.y);
			for(unsigned int i = 0; i < activatables.size(); i++) {
				if(activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect)) {
					//Check the distance to the rectangle
					while(pixel.x < map->GetTileWidth() && pixel.x >= 0 &&
							pixel.y < map->GetTileHeight() && pixel.y >= 0 &&
							!foundCol)
					{
						bool collide = activatables[i]->Contains(
							sf::Vector2f((float)(globTile.x*map->GetTileWidth() + pixel.x),
								(float)(globTile.y*map->GetTileHeight() + pixel.y)));
						//Once we find a colliding pixel, return its location
						if(collide) {
							int newNearest = tileRef * (horiz?map->GetTileWidth():map->GetTileHeight())
								+ pixelRef - step;
							if(abs(newNearest) < abs(nearest)) {
								nearest = newNearest;
								foundCol = true;
							}
						}
						pixelRef += step;
					}
				}
			}
		}

		//If we're going in a negative direction, we want to start at
		// the right of the tile rather than the left
		if(stepPos)
			pixelRef = 0;
		else
			pixelRef = (horiz?map->GetTileWidth():map->GetTileHeight()) - 1;
		tileRef += step;
	}

	return foundCol;
}

Level::Corner::FacingType Level::Corner::getFacingType(const sf::Vector2u &tileSize, const sf::Vector2f &pos) const {
	sf::Vector2f globPos(this->pos.x * tileSize.x, this->pos.y * tileSize.y);
	//Subtract one so we can use modulus correctly
	int quadrant = getQuadrant(globPos, pos)-1;
	int thisQuad = this->quadrant-1;
	FacingType ret;

	if(!inner) {
	   	if(thisQuad == quadrant) {
			ret = FACING_AWAY;
		} else if((thisQuad + 2)%4 == quadrant) {
			ret = FACING_CORNER;
		} else if((thisQuad + 3)%4 == quadrant) {
			ret = FACING_TANGENT_SECOND;
		} else if((thisQuad + 1)%4 == quadrant) {
			ret = FACING_TANGENT_FIRST;
		} else {
			// Equal - not in a quadrant, do some additional processing
			if((globPos.x == pos.x && (this->quadrant == 2 || this->quadrant == 4)) ||
					(globPos.y == pos.y && (this->quadrant == 1 || this->quadrant == 3))) {
				ret = FACING_TANGENT_FIRST;
				printf("aaa\n");
			}
			if((globPos.x == pos.x && (this->quadrant == 1 || this->quadrant == 3)) ||
					(globPos.y == pos.y && (this->quadrant == 2 || this->quadrant == 4))) {
				ret = FACING_TANGENT_SECOND;
				printf("bbb\n");
			}
		}
	} else {
		if(thisQuad == quadrant) {
			ret = FACING_CORNER;
		} else {
			ret = FACING_AWAY;
		}
	}

	return ret;
}

void Level::DoActivate()
{
	for(unsigned int i = 0; i < activatables.size(); i++) {
		/*if(activatables[i]->IsColliding(player)) {
			activatables[i]->Activate();
			didActivate = true;
		}*/
		bool wasCollidable = activatables[i]->IsCollidable();
		bool activated = activatables[i]->TryActivate(player);
		//If something activated and it was blocking light, then force an update of
		// all lights in the level
		if(wasCollidable && activated) {
			ForceUpdateLights();
		}
	}
}

void Level::ForceUpdateLights()
{
	for(unsigned int i = 0; i < lights.size(); i++) {
		lights[i]->ForceUpdate();
	}
}

void Level::AddLight(LightSource& light)
{
	lights.push_back(&light);
}

void Level::AddAttack(const AttackCone& attack)
{
	attackCones.push_back(attack);
}

sf::Vector2i Level::GetSize() const
{
	return sf::Vector2i(map->GetWidth()*map->GetTileWidth(), map->GetHeight()*map->GetTileHeight());
}

Player& Level::GetPlayer()
{
	return player;
}

const Player& Level::GetPlayer() const
{
	return player;
}

void Level::Update(const sf::Time& timePassed)
{
	for(unsigned int i = 0; i < activatables.size(); i++) {
		activatables[i]->Update(*this, timePassed);
		//Check for stairs, the only activatable that can be both active and finished
		/*if(activatables[i]->IsActive() && activatables[i]->IsFinished()) {
			//We've got to notify the Game that this level is done and we'd like to
			// transition to another.
			isActive = false;
		}*/
	}

	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->Update(*this, timePassed);
		/*if(player.IsColliding(*enemies[i])) {
			enemies[i]->Attack(player);
			//printf("Player colliding with enemy %d\n", i);
		}*/
	}

	//Iterate through all the player's attacks
	std::list<AttackCone>::iterator p = attackCones.begin();
	while(p != attackCones.end()) {
		//Grab p's current value into a temporary value and then increment p
		//We do this so that we delete the temp value, not p itself
		std::list<AttackCone>::iterator temp = p++;
		//Update the attack box
		temp->Update(*this, timePassed);
		if(temp->IsExpired()) {
			//If it's now expired, destroy it
			attackCones.erase(temp);
		} else if(&temp->GetOwner() == &player) {
			//Otherwise, check for collisions with enemies
			for(unsigned int i = 0; i < enemies.size(); i++) {
				if(!temp->IsMovableHit(*enemies[i]) && enemies[i]->IsColliding(*temp)) {
					temp->GetOwner().Attack(*enemies[i]);
					temp->MovableHit(*enemies[i]);
				}
			}
		} else {
			if(!temp->IsMovableHit(player) && player.IsColliding(*temp)) {
				temp->GetOwner().Attack(player);
				temp->MovableHit(player);
			}
		}
	}

	player.Update(*this, timePassed);

	//The owners of the lights should take care of updating them, we're just
	// going to draw them to the overlay texture
	lightTexture.clear(sf::Color());
	for(unsigned int i = 0; i < lights.size(); i++) {
		lightTexture.draw(*lights[i], sf::BlendMultiply);
		lights[i]->debugDraw(lightTexture, sf::BlendMode());
	}
	lightTexture.display();
}

void Level::SetHUDText(const std::string text)
{
	HUDText = text;
}

void Level::UpdateHUD(HUD& hud)
{
	player.UpdateHud(hud);
	if(!HUDText.empty()) {
		hud.changeText(HUDText);
		HUDText = "";
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	//Draw the tilemap
	target.draw(tilemapSprite, state);

	//Draw all the entities on top of the tiles
	for(unsigned int i = 0; i < activatables.size(); i++) {
		target.draw(*activatables[i], state);
	}
	for(unsigned int i = 0; i < enemies.size(); i++) {
		target.draw(*enemies[i], state);
	}
	//Uncomment to draw attack cones
	/*for(std::list<AttackCone>::const_iterator p = attackCones.begin(); p != attackCones.end(); p++) {
		sf::ConvexShape triangle = p->GetTriangle();
		triangle.setPosition(p->GetOwner().GetPos());
		target.draw(triangle, state);
	}*/

	//Draw the player
	target.draw(player, state);

	//Draw the light overlay on top of everything else
	target.draw(lightSprite, state);
}

bool Level::CheckLevelTransition(std::string& levelName, std::string& spawnName)
{
	if(!isActive) {
		levelName = nextLevel;
		spawnName = nextSpawn;
		return true;
	}
	return false;
}

void Level::SetLevelTransition(const std::string& levelName, const std::string& spawnName)
{
	nextLevel = levelName;
	nextSpawn = spawnName;
	isActive = false;
}
