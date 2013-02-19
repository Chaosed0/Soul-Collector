#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <sfml/Graphics.hpp>
#include <string>
using namespace std;

class Drawable: public sf::Drawable
{
private: 
	string name;			// name of the object
	bool hasDrawn;			
	bool isDiscovered;		// is the object discovered on screen
	sf::Sprite sprite;			
	sf::Texture texture;

public:
	Drawable(string fileLocation);
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif