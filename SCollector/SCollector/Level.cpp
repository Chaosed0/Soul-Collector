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
}

Level::Level(std::string mapName)
{
	lyrCollision = NULL;
	tsetCollision = NULL;

	Parse(mapName);
}

bool Level::Parse(std::string mapName)
{
	//Attempt to parse the map, and report any errors
	map.ParseFile(BASEMAPDIR + mapName);
	
	if(map.HasError())
	{
		fprintf(stderr, "Error %d: %s\n", map.GetErrorCode(), map.GetErrorText().c_str());
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
		if(tileset->GetName().compare("Collision") == 0)
		{
			//Save the tileset
			tsetCollision = tileset;		

			sf::Image imgCollision;
			//Load the image as an sf::Image
			if(!imgCollision.loadFromFile(BASEMAPDIR + tileset->GetImage()->GetSource()))
				fprintf(stderr, "Error loading image %d: %s\n", i, tileset->GetName().c_str());
			//Iterate through and figure out which pixels are transparent and not
			sf::Vector2u size = imgCollision.getSize();
			collisionMap.resize(size.x);

			for(unsigned int x = 0; x < size.x; x++)
			{
				collisionMap[x].resize(size.y);
				for(unsigned int y = 0; y < size.y; y++)
				{
					//The player should be able to collide with the tile if the
					// tile is not completely transparent
					collisionMap[x][y] = (imgCollision.getPixel(x, y).a > 0);
				}
			}
			//printf("(%d, %d): %d, %d\n", 33, 0, imgCollision.getPixel(33, 0).a, (collisionMap[33][0]==true));
		}
	}

	//We also need to find the collision layer
	for(int i = 0; i < map.GetNumLayers(); i++)
	{
		const Tmx::Layer *layer = map.GetLayer(i);
		//If this is the collision layer, store it
		if(layer->GetName().compare("Collision") == 0)
		{
			//Save the tileset
			lyrCollision = layer;		
		}
	}

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

	//Get the nearest tile in the layer corresponding to the position
	//BIG NOTE !!!!!!!!! -----------------------
	// We assume that tile 0 is unused because that's what GetTileId returns
	// when there's no tile. It also can be a valid tile id, though!

	int step;
	if(stepPos == true)
		step = 1;
	else
		step = -1;

	int layerHeight = lyrCollision->GetHeight();
	int layerWidth = lyrCollision->GetWidth();

	while(globTile.y >= 0 && globTile.y < layerHeight &&
			globTile.x >= 0 && globTile.x < layerWidth)
	{
		unsigned int tileIdx = lyrCollision->GetTileId(globTile.x, globTile.y);

		if(tileIdx != 0)
		{
			sf::Vector2i locTile = GetLocalTile(lyrCollision, globTile);
			while(pixel.x < map.GetTileWidth() && pixel.x >= 0 &&
					pixel.y < map.GetTileHeight() && pixel.y >= 0)
			{
				//printf("(%d, %d) -> (%d, %d)\n", globTile.x, globTile.y, locTile.x, locTile.y);
				//printf("(%d, %d) -> %s\n", locTile.x+pixel.x, locTile.y+i, (collisionMap[locTile.x + pixel.x][locTile.y + i]?"true":"false"));
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
	}
	return false;
}

/*bool Level::GetCollide(const sf::Vector2f& pos, const bool horiz, const bool stepPos, int& nearest) const
{
	//If either the collision tileset or layer is NULL, then don't do anything
	if(tsetCollision == NULL || lyrCollision == NULL)
		return false;

	//Figure out which direction to step
	int step;
	if(stepPos)
		step = 1;
	else
		step = -1;

	//We need to figure out which tile on the collision tileset this
	// position corresponds to on the layer
	int tileWidth = tsetCollision->GetTileWidth();
	int tileHeight = tsetCollision->GetTileHeight();
	sf::Vector2i globTile(pos.x / tileWidth, pos.y / tileHeight);
	sf::Vector2i pixel((int)pos.x % tileWidth, (int)pos.y % tileHeight);

	//Get the nearest tile in the layer corresponding to the position
	//BIG NOTE !!!!!!!!! -----------------------
	// We assume that tile 0 is unused because that's what GetTileId returns
	// when there's no tile.
	int layerWidth = lyrCollision->GetWidth();
	int layerHeight = lyrCollision->GetHeight();
	int tilesetWidth = tsetCollision->GetImage()->GetWidth() / tileWidth;
	sf::Vector2i nextTile = globTile;
	unsigned int tileIdx;

	//Figure out where the closest colliding tile in the specified direction is
	do
	{
		tileIdx = lyrCollision->GetTileId(nextTile.x, nextTile.y);
		//printf("tileIdx: %d, tile: (%d, %d)\n", tileIdx, nextTile.x, nextTile.y);
		if(horiz)
			nextTile.x += step;
		else
			nextTile.y += step;
	} while(tileIdx == 0 &&
			nextTile.x >= 0 && nextTile.x < layerWidth &&
			nextTile.y >= 0 && nextTile.y < layerHeight);

	//We still need nextTile, and it's one more step more than it should be
	if(horiz)
		nextTile.x -= step;
	else
		nextTile.y -= step;

	//Make sure there is a collision to check
	if(tileIdx != 0)
	{
		//Unless we're still on the same tile we started at, we want to
		// check from the beginning of the tile for the direction we're
		// checking (but not for the other direction)
		if(globTile != nextTile)
		{
			if(horiz && stepPos)
				pixel.x = 0;
			else if(horiz && !stepPos)
				pixel.x = 32;
			else if(!horiz && stepPos)
				pixel.y = 0;
			else
				pixel.y = 32;
		}

		//Do pixel-perfect collision on this tile
		//First, figure out which tile in the tileset it is
		sf::Vector2i locTile(tileIdx % tilesetWidth * tileWidth, tileIdx / tilesetWidth * tileHeight);
		bool collide = collisionMap[locTile.x+pixel.x][locTile.y+pixel.y];

		//Advance the pixel location until we hit something
		do
		{
			collide = collisionMap[locTile.x+pixel.x][locTile.y+pixel.y];
			if(horiz)
				pixel.x += step;
			else
				pixel.y += step;
		} while(!collide &&
				pixel.x >= 0 && pixel.x < tileWidth &&
				pixel.y >= 0 && pixel.y < tileHeight);

		if(horiz)
			pixel.x -= step;
		else
			pixel.y -= step;
		
		//Note: we subtract step from pixel here because we've gone one in too far;
		// we don't want the character to intersect with the block
		if(horiz)
			nearest = nextTile.x * tileWidth + pixel.x - step;
		else
			nearest = nextTile.y * tileHeight + pixel.y - step;
		
		//printf("locTile: (%d, %d) - Nearest: (%d, %d) + (%d, %d) %s%s \n", locTile.x, locTile.y, nextTile.x, nextTile.y, pixel.x, pixel.y, (stepPos?"+":"-"), (horiz?"x":"y"));
		//printf("Position: (%g, %g) vs nearest: %d\n", pos.x, pos.y, nearest);

		return collide;
	}
	//If there was no collision with any tiles on this axis, just return false
	else
		return false;
}*/

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
