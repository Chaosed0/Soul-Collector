/**
 * \brief Manages animations for entities.
 * \author Edward Lu
 * \date March 21 2013
 */

#ifndef __ANIMMANAGER_H
#define __ANIMMANAGER_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>

class SoundManager;

class AnimManager
{
public:

	/**
	 * Constructor. 
	 */
	AnimManager(const sf::IntRect& animBox, SoundManager& soundManager);

	/**
	 * Setter for sheet size.
	 * \param sheetSize The size of the sprite sheet this animation manager corresponds to.
	 */
	void SetSheetSize(sf::Vector2u sheetSize);

	/**
	 * Adds a set of frames to the animation sets of this entity.
	 * begin and end denote the number of frames to begin and end at;
	 * if the image wraps around, simply number them like so:
	 *
	 * 0 1 2 3
	 * 4 5 6 7
	 *
	 * NOTE: The animation set "idle" is added with begin==0, end==0
	 *  on the creation of an entity.
	 * \param animName The name of this animation.
	 * \param begin The first frame of the animation.
	 * \param end The last frame of the animation.
	 * \param loop Flag that makes the animation loop when set to true.
	 * \return True if the animation was successfully added, false otherwise.
	 */
	bool AddAnimSet(const std::string& animName, int begin, int end, bool loop = false);

	/**
	 * Changes the properties of an existing animation.
	 * If begin or end are set to <0, then they remain unchanged.
	 *
	 * \param animName The name of this animation.
	 * \param begin The first frame of the animation.
	 * \param end The last frame of the animation.
	 * \param loop Flag that makes the animation loop when set to true.
	 * \return True if the animation was successfully modified, false otherwise.
	 */
	bool ModifyAnimSet(const std::string& animName, int begin, int end, bool loop = false);
	
	/**
	 * Gets the current animation frame. 
	 * \return The sub-rectangle of the image where the current frame is located.
	 */
	sf::IntRect GetCurAnimRect();

	/**
	 * Plays an animation.
	 *
	 * If the animation is not currently the one that's playing, then StartAnim(anim)
	 *  is called; otherwise, StepAnim() is called.
	 * This ought to be called every time Update() is called, or the clock might get
	 *  out of sync.
	 * \param anim The name of the animation to play.
	 * \param timePassed The amount of time since the last time this function was called.
	 */
	void PlayAnim(const std::string& anim, const sf::Time& timePassed);

	/**
	 * Attaches a sound to an animation.
	 *
	 * When a frame of the animation is played, the corresponding sound is also played.
	 *  Multiple sounds can be attached to a given animation, in which case a random sound
	 *  will be played every time the animation is played.
	 * \param anim The name of the animation to attach the sound to.
	 * \param sound The name of the sound to attach, in soundManager.
	 * \return True if the animation exists, false otherwise.
	 */
	bool AttachSound(const std::string& anim, const std::string& sound);
private:
	/**
	 * Starts the entity playing the animation, if it exists.
	 * The animation set should have been added using AddAnimSet first.
	 * \param animName The name of the animation to start playing.
	 * \return True if the animation existed, false otherwise.
	 */
	bool StartAnim(const std::string& animName);

	/**
	 * Steps the current animation forward one frame.
	 * \return False if there is no current animation OR the current
	 *  animation finished; false otherwise.
	 */
	bool StepAnim();

	/** Size of each frame of the animation within the sprite sheet. */
	sf::IntRect animBox;
	/** Size of the sprite sheet. */
	sf::Vector2u sheetSize;
	/** The names of each animation, mapped to an animation identifier. */
	std::map<std::string, int> animNames;
	/** The first frame of the animation. */
	std::vector<int> animSetBegin;
	/** The last frame of the animation. */
	std::vector<int> animSetEnd;
	/** Whether or not animations should loop. */
	std::vector<bool> animLoop;
	/** Sounds associated with the animations. */
	std::vector<std::vector<std::string>> animSound;
	/** Current animation that is playing. */
	int curAnim;
	/** Frame of the current animation we are playing. */
	int curAnimFrame;
	/** The last animation identifier that was added to the map. */
	int lastAnimAdded;
	/** Total frames that are held within the sprite sheet. */
	int totalFrames;

	/** Time passed since last animation frame was played */
	sf::Time animTimer;
	/** Time between animation frames to wait */
	sf::Time animDelay;

	/** The default animation delay. */
	static const sf::Time defaultAnimDelay;

	/** Sound manager associated with this animation manager. */
	SoundManager& soundManager;
};

#endif