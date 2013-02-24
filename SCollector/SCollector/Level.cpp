/**
 * \author Chaosed0
 * \date 1/13/2013
 * \brief Implementation of level object
 */

#include "Level.h"

Level::Level()
{
	lyrCollision = NULL;
	tsetCollision = NULL;
	spawn.x = spawn.y = -1;
}

Level::Level(std::string mapName)
{
	lyrCollision = NULL;
	tsetCollision = NULL;
	spawn.x = spawn.y = -1;

	Parse(mapName);
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

			//Check the objects against our known object types...
			//Is the object a (the) spawn?
			if(object->GetType().compare("Spawn") == 0) {
				//Have we gotten a spawn already?
				if(spawn.x < 0 && spawn.y < 0) {
					spawn.x = object->GetX() + object->GetWidth()/2.0;
					spawn.y = object->GetY() - object->GetWidth()/2.0;
				}
				//If we've already found a spawnpoint, just ignore this one
			}
			//Here, we should do keys, traps, etc...
			//We don't know what type of object this is, issue an error
			else {
				fprintf(stderr, "WARNING: Found unknown object type %s\n", object->GetType());
			}
		}
	}

	printf("Map parsing complete\n");
	return true;
}

bool Level::AdjustY(const sf::Vector2f& pos, int& nearest) const
{
	//We need to figure out which tile on the collision tileset this
	// position corresponds to on the layer
	int tileWidth = tsetCollision->GetTileWidth();
	int tileHeight = tsetCollision->GetTileHeight();
	sf::Vector2i globTile(pos.x / tileWidth, pos.y / tileHeight);
	
	sf::Vector2i pixel((int)pos.x % tileWidth, (int)pos.y % tileHeight);

	//Get the nearest tile in the layer corresponding to the position
	//BIG NOTE !!!!!!!!! -----------------------
	// We assume that tile 0 is unused because that's what GetTileId returns
	// when there's no tile. It also can be a valid tile id, though!
	int layerHeight = lyrCollision->GetHeight();
	int tilesetWidth = tsetCollision->GetImage()->GetWidth() / tileWidth;

	while(globTile.y >= 0 && globTile.y < layerHeight)
	{
		unsigned int tileIdx = lyrCollision->GetTileId(globTile.x, globTile.y);
		sf::Vector2i locTile(tileIdx % tilesetWidth * tileWidth, tileIdx / tilesetWidth * tileHeight);
		for(int i = pixel.y; i >= 0; i--)
		{
			//printf("(%d, %d) -> (%d, %d)\n", globTile.x, globTile.y, locTile.x, locTile.y);
			//printf("(%d, %d) -> %s\n", locTile.x+pixel.x, locTile.y+i, (collisionMap[locTile.x + pixel.x][locTile.y + i]?"true":"false"));
			if(!collisionMap[locTile.x + pixel.x][locTile.y + i] || tileIdx == 0)
			{
				nearest = globTile.y*tileHeight + i;
				//printf(" ---- NEAREST = %d\n", nearest);
				return true;
			}
		}
		globTile.y--;
		pixel.y = 31;
	}
	return false;
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
	return sf::Vector2i(pos.x / map.GetTileWidth(), pos.y / map.GetTileHeight());
}

sf::Vector2i Level::GetPixel(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)pos.x % map.GetTileWidth(), (int)pos.y % map.GetTileHeight());
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

sf::Vector2f Level::GetSpawn() const
{
	return spawn;
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	std::vector<sf::Sprite> sprTilesetsConst = sprTilesets;

	//Render the layers of the map
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
						sprTilesetsConst[tilesetIdx].setTextureRect(sf::IntRect(locTile.x, locTile.y, map.GetTileWidth(), map.GetTileHeight()));
						//Set the tile's position to the correct place
						sprTilesetsConst[tilesetIdx].setPosition(x*map.GetTileWidth(), y*map.GetTileHeight());

						//Draw that tile
						target.draw(sprTilesetsConst[tilesetIdx], state);
					}
				}
			}
		}
	}
}
