
#include "SoundManager.h"

std::map<std::string, sf::SoundBuffer> SoundManager::soundBuffers;

SoundManager::SoundManager()
{
	lastSoundAdded = -1;
	lastSoundPlayed = -1;
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

		//Increment the sound ID counter
		lastSoundAdded++;
		//Add the sound to the library of sounds
		soundNames[soundName] = lastSoundAdded;
		soundLocs.push_back(soundLoc);
		soundLoop.push_back(loop);
		return true;
	}
	//If the sound was already registered, return false
	return false;
}

bool SoundManager::PlaySound(const std::string& soundName)
{
	std::map<std::string, int>::iterator soundNameIter = soundNames.find(soundName);
	if(soundNameIter != soundNames.end()) {
		sound.stop();
		sound.setLoop(soundLoop[soundNameIter->second]);
		sound.setBuffer(soundBuffers[soundLocs[soundNameIter->second]]);
		sound.play();
		return true;
	}
	return false;
}

bool SoundManager::StopSound(const std::string& soundName)
{
	sound.stop();
	return true;
}

void SoundManager::SetSoundVolume(const std::string& soundName, float volume)
{
}

void SoundManager::StopLastSound()
{
	sound.stop();
}