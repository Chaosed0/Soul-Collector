
#include "SoundManager.h"

std::map<std::string, sf::SoundBuffer> SoundManager::soundBuffers;

SoundManager::SoundManager()
{
	lastSoundAdded = 0;
}

bool SoundManager::AddSound(const std::string& soundLoc, const std::string& soundName, bool loop)
{
	//Ensure the sound is not already registered
	if(soundNames.find(soundName) == soundNames.end()) {
		//If the sound buffer has not already been registered, do so now
		if(soundBuffers.find(soundLoc) == soundBuffers.end()) {
			soundBuffers[soundLoc] = sf::SoundBuffer();
			soundBuffers[soundLoc].loadFromFile(soundLoc);
		}

		//Add the sound to the library of sounds
		soundNames[soundName] = lastSoundAdded;
		sounds.push_back(sf::Sound(soundBuffers[soundLoc]));
		sounds.back().setLoop(loop);
		//Increment the sound ID counter
		lastSoundAdded++;
		return true;
	}
	//If the sound was already registered, return false
	return false;
}

bool SoundManager::PlaySound(const std::string& soundName)
{
	if(soundNames.find(soundName) != soundNames.end()) {
		sounds[soundNames[soundName]].play();
		return true;
	}
	return false;
}

bool SoundManager::StopSound(const std::string& soundName)
{
	if(soundNames.find(soundName) != soundNames.end()) {
		sounds[soundNames[soundName]].stop();
		return true;
	}
	return false;
}