/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Implementation of level object
 */

#include "Level.h"
#include "Player.h"
#include "Key.h"
#include "Torch.h"
#include "Demon.h"
#include "Activatable.h"
#include "Movable.h"

Level::Level()
	: player(sf::Vector2f())
{
	lyrCollision = NULL;
	tsetCollision = NULL;
	spawn.x = spawn.y = -1;
}

Level::Level(std::string mapName)
	: player(sf::Vector2f())
{
	lyrCollision = NULL;
	tsetCollision = NULL;
	spawn.x = spawn.y = -1;

	Parse(mapName);

	//Now that we've got the map, draw it to a texture
	//Create the texture and attach the sprite to it
	tilemapTexture.create(map.GetWidth()*map.GetTileWidth(), map.GetHeight()*map.GetTileHeight());
	//Now draw to the texture
	for(int i = 0; i < map.GetNumLayers(); i++)
	{
		const Tmx::Layer *layer = map.GetLayer(i);

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
					//const Tmx::Tileset *tileset = map.GetTileset(tilesetIdx);

					//Map the tile index to a coordinate in the tileset
					//unsigned int tileIdx = layer->GetTileId(x, y);
					sf::Vector2i locTile = GetLocalTile(layer, sf::Vector2i(x, y));

					if(locTile.x >= 0 && locTile.y >= 0)
					{
						//Get the part of the sprite corresponding to the tile
						sprTilesets[tilesetIdx].setTextureRect(sf::IntRect(locTile.x, locTile.y, map.GetTileWidth(), map.GetTileHeight()));
						//Set the tile's position to the correct place
						sprTilesets[tilesetIdx].setPosition((float)x*map.GetTileWidth(), (float)y*map.GetTileHeight());

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

	//Create the light overlay texture and sprite
	lightTexture.create(map.GetWidth()*map.GetTileWidth(), map.GetHeight()*map.GetTileHeight());
	lightTexture.clear();
	lightTexture.display();
	lightSprite.setTexture(lightTexture.getTexture());
	//Add the player's light
	player.AddLight(*this);

	//Set the player's position to the spawn
	player.SetPos(spawn);
}

bool Level::Parse(std::string mapName)
{
	printf("Parsing map...\n");
	//Attempt to parse the map, and report any errors
	map.ParseFile(BASEMAPDIR + mapName);
	
	if(map.HasError())
	{
		fprintf(stderr, "WARNING: Could not parse level: %s\n", map.GetErrorText().c_str());
		return false;
	}

	//Make the tilesets of the map into sprites
	texTilesets.resize(map.GetNumTilesets());
	sprTilesets.resize(map.GetNumTilesets());
	for(int i = 0; i < map.GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map.GetTileset(i);
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
	for(int i = 0; i < map.GetNumLayers(); i++) {
		const Tmx::Layer *layer = map.GetLayer(i);
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
	for (int i = 0; i < map.GetNumObjectGroups(); ++i) {
		//Get an object group.
		const Tmx::ObjectGroup *objectGroup = map.GetObjectGroup(i);

		//Iterate through all objects in the object group
		for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
			//Grab objects from the group
			const Tmx::Object *object = objectGroup->GetObject(j);
			const std::string &type = object->GetType();

			//Check the objects against our known object types...
			//Is the object a (the) spawn?
			if(type.compare("Spawn") == 0) {
				//Have we gotten a spawn already?
				if(spawn.x < 0 && spawn.y < 0) {
					spawn.x = object->GetX() + object->GetWidth()/2.0f;
					spawn.y = object->GetY() - object->GetWidth()/2.0f;
				}
				//If we've already found a spawnpoint, just ignore this one
			}
			else if(type.compare("Key") == 0) {
				/*std::string door = object->GetProperties().GetLiteralProperty("Door");
				if(door.empty()) {
					fprintf(stderr, "WARNING: Key has no associated door! Ignored...");
				} else {*/
					activatables.push_back(new Key(sf::Vector2f(object->GetX() + object->GetWidth()/2.0f,
						object->GetY() - object->GetHeight()/2.0f)));
				//}
			}
			else if(type.compare("Torch") == 0) {
				activatables.push_back(new Torch(sf::Vector2f(object->GetX() + object->GetWidth()/2.0f,
					object->GetY() - object->GetHeight()/2.0f)));
				//This is a bad thing and I feel bad for doing it
				((Torch*)activatables.back())->AddLight(*this);
			}
			else if(type.compare("Demon") == 0) {
				enemies.push_back(new Demon(sf::Vector2f(object->GetX() + object->GetWidth()/2.0f,
					object->GetY() - object->GetHeight()/2.0f)));
			}
			//Here, we should do keys, traps, etc...
			//We don't know what type of object this is, issue an error
			else {
				fprintf(stderr, "WARNING: Found unknown object type %s\n", object->GetType().c_str());
			}
		}
	}

	if(spawn.x < 0 || spawn.y < 0) {
		printf("WARNING: No spawnpoint found! Defaulting to (0, 0)...\n");
		spawn = sf::Vector2f(0, 0);
	}


	printf("Map parsing complete\n");
	return true;
}

const Tmx::Tileset* Level::GetTileset(const Tmx::Layer* layer, const sf::Vector2i& globTile) const
{
	int tilesetIdx = layer->GetTileTilesetIndex(globTile.x, globTile.y);
	return (tilesetIdx < 0 ? NULL : map.GetTileset(tilesetIdx));
}

sf::Vector2i Level::GetLocalTile(const Tmx::Layer* layer, const sf::Vector2i& globTile) const
{
	const Tmx::Tileset* tileset = GetTileset(layer, globTile);
	int tileWidth = map.GetTileWidth();
	int tileHeight = map.GetTileHeight();
	int tilesetWidth = tileset->GetImage()->GetWidth() / tileWidth;
	int tileIdx = layer->GetTileId(globTile.x, globTile.y);

	if(tileIdx != 0)
		return sf::Vector2i(tileIdx % tilesetWidth * tileWidth, tileIdx / tilesetWidth * tileHeight);
	else
		return sf::Vector2i(-1, -1);
}

sf::Vector2i Level::GetGlobalTile(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)pos.x / map.GetTileWidth(), (int)pos.y / map.GetTileHeight());
}

sf::Vector2i Level::GetGlobalTileBR(const sf::Vector2f& pos) const
{
	return sf::Vector2i(((int)pos.x-1) / map.GetTileWidth(), ((int)pos.y - 1) / map.GetTileHeight());
}

sf::Vector2i Level::GetPixel(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)pos.x % map.GetTileWidth(), (int)pos.y % map.GetTileHeight());
}

bool Level::GetCollide(const sf::Vector2f& pos, float angle, sf::Vector2f& nearest) const
{
	while(angle < 0) {
		angle += 2*PI;
	}
	float tanDir = tan(angle);
	//printf("%g\n", tanDir);

	//The first iteration is different from the others because we
	// need to figure out how far away the first block is
	sf::Vector2f nearestX((float)GetGlobalTile(pos).x*map.GetTileWidth(),
		(float)GetGlobalTile(pos).y*map.GetTileHeight());
	float distX = FLT_MAX, distY = FLT_MAX;
	bool foundX = false, foundY = false;

	//printf("X DIRECTION\n\n");

	//GetGlobalTile says tiles include their top-left edges; we have to
	// fix that for certain lines
	if(angle > PI/2.0f && angle < 3*PI/2.0f) {
		nearestX.x+=31;
	} 
	if(angle > PI && angle < 2*PI) {
		nearestX.y+=31;
	}

	while(nearestX.x > 0 && nearestX.x < map.GetWidth()*map.GetTileWidth() &&
			nearestX.y > 0 && nearestX.y < map.GetHeight()*map.GetTileHeight()) {
		sf::Vector2i globTile = GetGlobalTile(nearestX);
		if(lyrCollision->GetTileId(globTile.x, globTile.y) > 0) {
			sf::Vector2f relDist = pos - nearestX;
			distX = magnitude(relDist);
			foundX = true;
			break;
		}

		if(!(angle > PI/2.0f && angle < 3*PI/2.0f)) {
			nearestX.x += map.GetTileWidth();
		} else {
			nearestX.x -= map.GetTileWidth();
		}
		nearestX.y = pos.y + (tanDir*(nearestX.x - pos.x));
		//printf("Nearest: (%g, %g)\n", nearest.x, nearest.y);
	}

	sf::Vector2f nearestY((float)GetGlobalTile(pos).x*map.GetTileWidth(),
		(float)GetGlobalTile(pos).y*map.GetTileHeight());

	//GetGlobalTile says tiles include their top-left edges; we have to
	// fix that for certain lines
	if(angle > PI/2.0f && angle < 3*PI/2.0f) {
		nearestY.x += 31;
	} 
	if(angle > PI && angle < 2*PI) {
		nearestY.y += 31;
	}

	//printf("Y DIRECTION\n\n");

	while(nearestY.y > 0 && nearestY.y < map.GetHeight()*map.GetTileHeight() &&
			nearestY.x > 0 && nearestY.x < map.GetWidth()*map.GetTileHeight()) {
		sf::Vector2i globTile = GetGlobalTile(nearestY);
		if(lyrCollision->GetTileId(globTile.x, globTile.y) > 0) {
			sf::Vector2f relDist = pos - nearestY;
			distY = magnitude(relDist);
			foundY = true;
			break;
		}

		if(angle > 0.0f && angle < PI) {
			nearestY.y += map.GetTileHeight();
		} else {
			nearestY.y -= map.GetTileHeight();
		}
		nearestY.x = (nearestY.y - pos.y) / tanDir + pos.x;
		//printf("Nearest: (%g, %g)\n", nearest.x, nearest.y);
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
	//We need to figure out which tile on the collision tileset this
	// position corresponds to on the layer
	sf::Vector2i globTile = GetGlobalTile(pos);
	sf::Vector2i pixel = GetPixel(pos);

	int layerHeight = lyrCollision->GetHeight();
	int layerWidth = lyrCollision->GetWidth();

	//Get the nearest tile in the layer corresponding to the position
	//BIG NOTE !!!!!!!!! -----------------------
	// We assume that tile 0 is unused because that's what GetTileId returns
	// when there's no tile. It also can be a valid tile id, though!

	int step;
	if(stepPos == true)
		step = 1;
	else
		step = -1;

	if(horiz)
	{
		globTile.x += step;
		if(stepPos)
			pixel.x = 0;
		else
			pixel.x = map.GetTileWidth() - 1;
	}
	else
	{
		globTile.y += step;
		if(stepPos)
			pixel.y = 0;
		else
			pixel.y = map.GetTileHeight() - 1;
	}

	//printf("(%d, %d)\n", globTile.x, globTile.y);
	//printf("(%d, %d)\n", layerWidth, layerHeight);

	if(globTile.y >= 0 && globTile.y < layerHeight &&
			globTile.x >= 0 && globTile.x < layerWidth)
	{
		unsigned int tileIdx = lyrCollision->GetTileId(globTile.x, globTile.y);
		//printf("(%d, %d) %d\n", globTile.x, globTile.y, tileIdx);

		if(tileIdx != 0)
		{
			sf::Vector2i locTile = GetLocalTile(lyrCollision, globTile);
			while(pixel.x < map.GetTileWidth() && pixel.x >= 0 &&
					pixel.y < map.GetTileHeight() && pixel.y >= 0)
			{
				//printf("(%d, %d) -> (%d, %d)\n", globTile.x, globTile.y, locTile.x, locTile.y);
				bool collide = collisionMap[locTile.x + pixel.x][locTile.y + pixel.y];
				if(collide)
				{
					if(horiz)
						nearest = globTile.x * map.GetTileWidth() + pixel.x - step;
					else
						nearest = globTile.y * map.GetTileHeight() + pixel.y - step;
					//printf("(%d, %d) -> %u\n", globTile.x, globTile.y, tileIdx);
					//printf("(%d, %d), %d\n", pixel.x, pixel.y, nearest);
					//printf(" ---- NEAREST = %d\n", nearest);
					return true;
				}
				
				if(horiz)
					pixel.x += step;
				else
					pixel.y += step;
			}
		}
	}

	nearest = (stepPos?INT_MAX:-INT_MAX);
	return false;
}

bool Level::DoActivate()
{
	bool didActivate = false;

	for(unsigned int i = 0; i < activatables.size(); i++) {
		if(activatables[i]->IsColliding(player)) {
			activatables[i]->Activate();
			printf("Activated activatable %d\n", i);
			didActivate = true;
		}
	}
	
	return didActivate;
}

void Level::AddLight(const LightSource& light)
{
	lights.push_back(&light);
}

sf::Vector2i Level::GetSize() const
{
	return sf::Vector2i(map.GetWidth()*map.GetTileWidth(), map.GetHeight()*map.GetTileHeight());
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
	}

	for(unsigned int i = 0; i < enemies.size(); i++) {
		enemies[i]->Update(*this, timePassed);
		if(player.IsColliding(*enemies[i]) || enemies[i]->IsColliding(player)) {
			printf("Player colliding with enemy %d\n", i);
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

	//Draw the player
	target.draw(player, state);

	//Draw the light overlay on top of everything else
	target.draw(lightSprite, state);
}
