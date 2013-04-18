/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Implementation of level object
 */

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

Level::Level(Player& player)
	: player(player)
{
	mapSize = sf::Vector2f(0, 0);
	tileSize = sf::Vector2f(0, 0);
	lyrCollision = NULL;
	tsetCollision = NULL;
	isActive = false;
}

Level::Level(Player& player, const std::string& mapName, const std::string& spawnName)
	: player(player)
{
	mapSize = sf::Vector2f(0, 0);
	tileSize = sf::Vector2f(0, 0);
	lyrCollision = NULL;
	tsetCollision = NULL;

	LoadMap(mapName, spawnName);
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

	//Spawn the player
	SpawnPlayer(spawnName);

	//The level is now active and can be used.
	isActive = true;
	
	return true;
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
	playerAttacks.clear();
	enemyAttacks.clear();
	//Clear the collision map
	collisionMap.clear();
	texTilesets.clear();
	sprTilesets.clear();
	//Don't delete the collision tileset/layer, just set them to null
	tsetCollision = NULL;
	lyrCollision = NULL;
	//Clear active stairs
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
	mapSize = sf::Vector2f(map->GetWidth(), map->GetHeight());
	tileSize = sf::Vector2f(map->GetTileWidth(), map->GetTileHeight());
	
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
			sf::Vector2f objectPos(object->GetX() + object->GetWidth()/2.0f,
						object->GetY() - object->GetHeight()/2.0f);

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
				if(door.empty()) {
					fprintf(stderr, "WARNING: Key has no associated door! Ignored...");
				} else {
					activatables.push_back(new Key(objectPos,door));
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
				activatables.push_back(new Door(objectPos, object->GetName()));
				activatables.back()->SetRot(object->GetRot());
			}
			//We don't know what type of object this is, issue an error
			else {
				fprintf(stderr, "WARNING: Found unknown object type %s\n", object->GetType().c_str());
			}
		}
	}

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
	return sf::Vector2i((int)pos.x / map->GetTileWidth(), (int)pos.y / map->GetTileHeight());
}

sf::Vector2i Level::GetPixel(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)pos.x % map->GetTileWidth(), (int)pos.y % map->GetTileHeight());
}

bool Level::GetColliding(const sf::Vector2i& locTile, const sf::Vector2i& pixel) const
{
	return collisionMap[locTile.x + pixel.x][locTile.y + pixel.y];
}

bool Level::GetCollide(const sf::Vector2f& pos, float angle, sf::Vector2f& nearest) const
{
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
	while(nearestX.x >= 0 && nearestX.x < map->GetWidth()*map->GetTileWidth() &&
			nearestX.y >= 0 && nearestX.y < map->GetHeight()*map->GetTileHeight() &&
			!foundX) {
		int globTileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		const Tmx::Tile* tile = tsetCollision->GetTile(globTileId);

		//List of entities to also check for collisions
		foundObj = false;
		sf::IntRect rect(globTile.x*tileSize.x, globTile.y*tileSize.y, tileSize.x, tileSize.y);
		for(unsigned int i = 0; i < activatables.size() && !foundObj; i++) {
			foundObj = activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect);
		}

		//If the tile is identified as colliding with light or we have an entity
		// to look for
		int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		if((tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true))
				|| foundObj) {
			sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
			sf::Vector2i pixel(nearestX.x-globTile.x*tileSize.x,
				nearestX.y-globTile.y*tileSize.y);
			//Check the pixels of the tile along this line
			while(pixel.x >= 0 && pixel.x < tileSize.x &&
					pixel.y >= 0 && pixel.y < tileSize.y &&
					!foundX) {

				//If the pixel of the tile is colliding, then we have found a colliding tile
				if(tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true)) {
					foundX = GetColliding(locTile, pixel);
				}
				for(unsigned int i = 0; i < activatables.size(); i++) {
					foundX = foundX || activatables[i]->IsCollidable() && activatables[i]->Contains(nearestX);
				}

				if(!foundX) {
					//If this pixel is not colliding, then keep searching
					(negXDir?nearestX.x--:nearestX.x++);
					(negYDir?nearestX.y-=abs(tanDir):nearestX.y+=abs(tanDir));
					pixel.x = nearestX.x-globTile.x*tileSize.x;
					pixel.y = nearestX.y-globTile.y*tileSize.y;
				} else {
					//Otherwise, note down where we found this and break
					//Back out one pixel; the previous pixel was the last non-colliding one
					(negXDir?nearestX.x++:nearestX.x--);
					(negYDir?nearestX.y+=abs(tanDir):nearestX.y-=abs(tanDir));
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

	while(nearestY.y >= 0 && nearestY.y < map->GetHeight()*map->GetTileHeight() &&
			nearestY.x >= 0 && nearestY.x < map->GetWidth()*map->GetTileHeight() &&
			!foundY) {
		int globTileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		const Tmx::Tile* tile = tsetCollision->GetTile(globTileId);

		//List of entities to also check for collisions
		foundObj = false;
		sf::IntRect rect(globTile.x*tileSize.x, globTile.y*tileSize.y, tileSize.x, tileSize.y);
		for(unsigned int i = 0; i < activatables.size() && !foundObj; i++) {
			foundObj = activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect);
		}

		int tileId = lyrCollision->GetTileId(globTile.x, globTile.y);
		if((tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true))
				|| foundObj) {
			sf::Vector2i locTile(GetLocalTile(lyrCollision, globTile));
			sf::Vector2i pixel(nearestY.x-globTile.x*tileSize.x,
				nearestY.y-globTile.y*tileSize.y);
			//Check the pixels of the tile along this line
			while(pixel.x >= 0 && pixel.x < tileSize.x &&
					pixel.y >= 0 && pixel.y < tileSize.y &&
					!foundY) {

				//Check if this tile is colliding with the ray, if we even have a tile to check
				if(tileId > 0 && (tile != NULL?!tile->GetProperties().HasProperty("Clear"):true)) {
					foundY = GetColliding(locTile, pixel);
				}
				for(unsigned int i = 0; i < activatables.size(); i++) {
					foundY = foundY || activatables[i]->IsCollidable() && activatables[i]->Contains(nearestY);
				}

				if(!foundY) {
					//If this pixel is not colliding, then keep searching
					(negYDir?nearestY.y--:nearestY.y++);
					(negXDir?nearestY.x-=1/abs(tanDir):nearestY.x+=1/abs(tanDir));
					pixel.x = nearestY.x-globTile.x*tileSize.x;
					pixel.y = nearestY.y-globTile.y*tileSize.y;
				} else {
					//Otherwise, note down where we found this and break
					(negYDir?nearestY.y++:nearestY.y--);
					(negXDir?nearestY.x+=1/abs(tanDir):nearestY.x-=1/abs(tanDir));
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

	float dist = std::min(distX, distY);

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
						float newNearest = tileRef * (horiz?map->GetTileWidth():map->GetTileHeight())
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
			sf::IntRect rect(globTile.x*tileSize.x, globTile.y*tileSize.y,tileSize.x, tileSize.y);
			for(unsigned int i = 0; i < activatables.size(); i++) {
				if(activatables[i]->IsCollidable() && activatables[i]->IsColliding(rect)) {
					//Check the distance to the rectangle
					while(pixel.x < map->GetTileWidth() && pixel.x >= 0 &&
							pixel.y < map->GetTileHeight() && pixel.y >= 0 &&
							!foundCol)
					{
						bool collide = activatables[i]->Contains(
							sf::Vector2f(globTile.x*map->GetTileWidth() + pixel.x,
							globTile.y*map->GetTileHeight() + pixel.y));
						//Once we find a colliding pixel, return its location
						if(collide) {
							float newNearest = tileRef * (horiz?map->GetTileWidth():map->GetTileHeight())
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

void Level::DoActivate()
{
	for(unsigned int i = 0; i < activatables.size(); i++) {
		/*if(activatables[i]->IsColliding(player)) {
			activatables[i]->Activate();
			didActivate = true;
		}*/
		activatables[i]->TryActivate(player);
	}
}

void Level::AddLight(const LightSource& light)
{
	lights.push_back(&light);
}

void Level::AddAttack(const AttackCone& attack, bool enemy)
{
	if(enemy) {
		enemyAttacks.push_back(attack);
	} else {
		playerAttacks.push_back(attack);
	}
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
	player.Update(*this, timePassed);
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
		if(player.IsColliding(*enemies[i]) || enemies[i]->IsColliding(player)) {
			enemies[i]->Attack(player);
			//printf("Player colliding with enemy %d\n", i);
		}
	}

	//Iterate through all the player's attacks
	std::list<AttackCone>::iterator p = playerAttacks.begin();
	while(p != playerAttacks.end()) {
		//Grab p's current value into a temporary value and then increment p
		//We do this so that we delete the temp value, not p itself
		std::list<AttackCone>::iterator temp = p++;
		//Update the attack box
		temp->Update(*this, timePassed);
		if(temp->IsExpired()) {
			//If it's now expired, destroy it
			playerAttacks.erase(temp);
		} else {
			//Otherwise, check for collisions with enemies
			for(unsigned int i = 0; i < enemies.size(); i++) {
				if(!temp->IsMovableHit(player) && enemies[i]->IsColliding(*temp)) {
					player.Attack(*enemies[i]);
					temp->MovableHit(player);
				}
			}
		}
	}

	//Iterate through all the monsters' attacks
	p = enemyAttacks.begin();
	while(p != enemyAttacks.end()) {
		//Grab p's current value into a temporary value and then increment p
		//We do this so that we delete the temp value, not p itself
		std::list<AttackCone>::iterator temp = p++;
		//Update the attack box
		temp->Update(*this, timePassed);
		if(temp->IsExpired()) {
			//If it's now expired, destroy it
			enemyAttacks.erase(temp);
		} else {
			//Otherwise, check for collisions with enemies
			for(unsigned int i = 0; i < enemies.size(); i++) {
				if(player.IsColliding(*temp)) {
					enemies[i]->Attack(player);
				}
			}
		}
	}

	//The owners of the lights should take care of updating them, we're just
	// going to draw them to the overlay texture
	lightTexture.clear(sf::Color());
	for(unsigned int i = 0; i < lights.size(); i++) {
		lightTexture.draw(*lights[i], sf::BlendMultiply);
	}
	lightTexture.display();
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
	for(std::list<AttackCone>::const_iterator p = playerAttacks.begin(); p != playerAttacks.end(); p++) {
		sf::ConvexShape triangle = p->GetTriangle();
		triangle.setPosition(player.GetPos());
		target.draw(triangle, state);
	}

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