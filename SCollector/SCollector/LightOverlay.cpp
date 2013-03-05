
#include "LightOverlay.h"
#include "Level.h"

LightOverlay::LightOverlay(int rays, int radius, const sf::View& view)
{
	this->rays = rays;
	this->radius = radius;

	//Init the image and the texture, just temporary
	overlayTexture.create(view.getSize().x, view.getSize().y);
	overlaySprite.setTexture(overlayTexture.getTexture());
	overlaySprite.setOrigin(overlayTexture.getSize().x/2.0f, overlayTexture.getSize().y/2.0f);

	circleImage.create(radius*2, radius*2, sf::Color());
	sf::Vector2u imgSize = circleImage.getSize();

	for(int x = 0; x < imgSize.x; x++) {
		for(int y = 0; y < imgSize.y; y++) {
			sf::Vector2f relDist = sf::Vector2f(x - radius, y - radius);
			float dist = sqrt(relDist.x*relDist.x + relDist.y*relDist.y);
			if(dist > radius) {
				circleImage.setPixel(x, y, sf::Color());
			} else {
				circleImage.setPixel(x, y, sf::Color(0, 0, 0, 255*(dist)/(radius)));
				//printf("point: (%d, %d), dist: %g, ratio: %g\n", x, y, dist, dist/radius);
			}
		}
	}

	circleTexture.loadFromImage(circleImage);
	circleSprite.setTexture(circleTexture);
	circleSprite.setOrigin(radius, radius);
	blackRect.setSize(view.getSize());
	blackRect.setFillColor(sf::Color());

	font.loadFromFile("assets/fonts/LiberationMono-Regular.ttf");
}

void LightOverlay::Update(const Level& level, const sf::View& view)
{

	circles.clear();
	texts.clear();
	sf::Vector2f playerPos = level.GetPlayer().GetPos();
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f viewPos = view.getCenter();

	//Redraw the light circle, finding the distances to nearest walls at angles
	// When drawing, the two constrants are thus:
	// 1. The (x, y) coordinate of the arc may not go above the smaller ray
	// 2. The distance may not go above the dist of the larger ray

	/*sf::Vector2f point;
	bool found = level.GetCollide(level.GetPlayer().GetPos(), 17*3.14159/16.0, point);
	circles.push_back(sf::CircleShape(2.0f));
	circles.back().setPosition(point);*/

	float angle1 = 0;
	float angle2 = 2*3.14159/(float)rays;
	sf::Vector2f point1, point2;
	bool found1, found2;
	float minDist;
	sf::Vector2u imgSize = circleImage.getSize();
	found1 = level.GetCollide(level.GetPlayer().GetPos(), angle1, point1);
	found2 = level.GetCollide(level.GetPlayer().GetPos(), angle2, point2);


	for(int i = 1; i < rays; i++) {
		circles.push_back(sf::CircleShape(2.0f));
		circles.back().setPosition(point1);
		std::stringstream sstream;
		sstream << angle1;
		texts.push_back(sf::Text(sf::String(sstream.str()), font, 12));
		texts.back().setPosition(point1);

		found1 = found2;
		point1 = point2;
		angle1 = angle2;
		angle2 = 2.0f*3.14159/(float)rays * (i+1);
		found2 = level.GetCollide(level.GetPlayer().GetPos(), angle2, point2);
	}

	//Note: Since the circle's being drawn to "view coordinates" instead of "global coordinates",
	// we've got to correct the player's position if we want the circle to be drawn on top of
	// the player
	//Theory behind this bit is that view coordinates are just global coordinates translated;
	// the amount of the translation is the distance from the global axes to the view axes
	sf::Vector2f viewcorrection(viewPos.x - viewSize.x/2, viewPos.y - viewSize.y/2);
	//Also, for some reason view coordinates have a reversed y-axis... ?_?
	circleSprite.setPosition(sf::Vector2f(playerPos.x - viewcorrection.x, viewSize.y-(playerPos.y - viewcorrection.y)));

	//printf("playerpos: (%g, %g), correction:(%g, %g)\n", playerPos.x, playerPos.y, correctpos.x, correctpos.y);

	//Update the overlay rectangle to follow the view
	//rect.setPosition(view.getCenter());
	overlaySprite.setPosition(view.getCenter());

	overlayTexture.clear(sf::Color(0,0,0,0));
	overlayTexture.draw(blackRect);

#ifdef LINUX
	overlayTexture.draw(circleSprite, sf::RenderStates(sf::BlendNone));
#else
	overlayTexture.draw(circleSprite, sf::RenderStates(sf::BlendMode::BlendNone));
#endif
}

void LightOverlay::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(overlaySprite, state);
	for(int i = 0; i < circles.size(); i++)
	{
		target.draw(circles[i]);
		target.draw(texts[i]);
	}
}