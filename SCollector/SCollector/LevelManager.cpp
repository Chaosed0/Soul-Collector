
#include "LevelManager.h"
#include "Player.h"

LevelManager::LevelManager()
	: player(sf::Vector2f(0, 0))
{
	curLevelName = "";
}

LevelManager::LevelManager(const std::string& mapName, const std::string& spawnName)
	: player(sf::Vector2f(0, 0))
{
	LoadMap(mapName, spawnName);
}

bool LevelManager::LoadMap(const std::string& mapName, const std::string& spawnName)
{
	bool loadMapResult = true;
	std::pair<std::map<std::string, Level*>::iterator, bool> insertPair;
	insertPair = levels.insert(std::pair<std::string, Level*>(mapName, (Level*)NULL));
	if(insertPair.second) {
		//If we did actually insert a level, we need to initialize it
		insertPair.first->second = new Level(player);
		if(insertPair.first->second->LoadMap(mapName, spawnName) == false) {
			//If initialization fails, undo everything we just did and return false
			delete insertPair.first->second;
			levels.erase(insertPair.first);
			fprintf(stderr, "WARNING: Could not load map %s\n", mapName.c_str());
			return false;
		}
	} else {
		//If the level was found, simply change the player's location
		insertPair.first->second->SpawnPlayer(spawnName);
	}
	//Now, set the current level to the one we just loaded
	curLevelName = insertPair.first->first;
	//Set that level to active
	GetCurrentLevel().SetActive(true);
	return loadMapResult;
}

bool LevelManager::Update(const sf::Time& timePassed)
{
	GetCurrentLevel().Update(timePassed);

	//Check if we need to transition levels
	std::string levelName;
	std::string spawnName;
	bool transition;
	transition = GetCurrentLevel().CheckLevelTransition(levelName, spawnName);
	//If we need to change levels, do so
	if(transition) {
		LoadMap(levelName, spawnName);
	}
	return true;
}
void LevelManager::Reset()
{
	player.Reset();
	for(std::map<std::string, Level*>::iterator p = levels.begin();
			p != levels.end(); p++) {
		delete p->second;
	}
	levels.clear();
	curLevelName = "";
}

Player& LevelManager::GetPlayer()
{
	return player;
}

bool LevelManager::HasLoadedLevel()
{
	return (curLevelName.compare("") != 0);
}

Level& LevelManager::GetCurrentLevel()
{
	return *levels.find(curLevelName)->second;
}
