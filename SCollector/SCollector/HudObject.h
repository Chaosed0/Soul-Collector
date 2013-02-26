#include <SFML/Graphics.hpp>

class HudObject:public sf::Drawable
{
private:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape rect;
	int filled;

public:

	HudObject();
	HudObject(std::string image, float xPos, int winHeight);

	float getWidth();

	void changeFill(float delta);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};