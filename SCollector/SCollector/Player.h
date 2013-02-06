/**
 * \brief Represents the player's avatar.
 * \author Edward Lu
 * \date January 28 2013
 *
 * This is a subclass of sf::Drawable so that it can be drawn by itself.
 */

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable
{
//Public attributes; visible to everyone
public:
	/**
	 * Init constructor.
	 * Initializes the player with no graphical component whatsoever - Make sure to
	 *  call LoadTexture() before drawing the player!
	 */
	Player(sf::Vector2f pos);

	/**
	 * Init constructor.
	 * Equivalent to calling the default constructor, then LoadTexture().
	 * \param pathToImage The path in the filesystem where the image is.
	 */
	Player(sf::Vector2f pos, std::string pathToImage);

	/**
	 * Loads a texture that represents the player.
	 * \param pathToImage The path in the filesystem where the image is.
	 */
	void LoadTexture(std::string pathToImage);

	/**
	 * Starts or stops the player moving left.
	 * \param start If true, the player starts moving left; otherwise, they stop
	 *  moving left.
	 */
	void MoveLeft(bool start);
	/**
	 * Starts or stops the player moving right.
	 * \param start If true, the player starts moving right; otherwise, they stop
	 *  moving right.
	 */
	void MoveRight(bool start);
	/**
	 * Starts or stops the player moving up.
	 * \param start If true, the player starts moving up; otherwise, they stop
	 *  moving up.
	 */
	void MoveUp(bool start);
	/**
	 * Starts or stops the player moving down.
	 * \param start If true, the player starts moving down; otherwise, they stop
	 *  moving down.
	 */
	void MoveDown(bool start);

	/**
	 * Updates the player's position.
	 * This function should be called once per frame.
	 */
	void Update();

//Private attributes; visible only within this class
private:
	/** Inherited from sf::Drawable */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	/** Initialization procedures that are shared between the constructors of the player */
	void Init(sf::Vector2f pos);

	/** Texture representing the player */
	sf::Texture texture;
	/** Sprite representing the player */
	sf::Sprite sprite;

	/** Flags set to true when the player should move in the corresponding direction */
	bool moveLeft, moveRight, moveUp, moveDown;

	/** Movement speed of the player */
	float moveSpeed;
};