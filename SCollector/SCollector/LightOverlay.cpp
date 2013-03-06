
#include "LightOverlay.h"
#include "Level.h"

LightOverlay::LightOverlay(int rays, int radius, const sf::View& view)
	: blackRect(view.getSize())
{
	this->rays = rays;
	this->radius = radius;

	//Init the image and the texture, just temporary
	overlayTexture.create(view.getSize().x, view.getSize().y);
	overlaySprite.setTexture(overlayTexture.getTexture());
	overlaySprite.setOrigin(overlayTexture.getSize().x/2.0f, overlayTexture.getSize().y/2.0f);
	
	circleOverlay.create(view.getSize().x, view.getSize().y);
	circleOverlaySprite.setTexture(circleOverlay.getTexture());
	circleOverlaySprite.setPosition(0, 0);

	circleImage.loadFromFile("assets/img/LightAura.png");
	circleTexture.loadFromImage(circleImage);
	circleSprite.setTexture(circleTexture);
	circleSprite.setScale(sf::Vector2f(radius*2.0f/circleTexture.getSize().x,
							radius*2.0f/circleTexture.getSize().y));
	circleSprite.setOrigin(circleTexture.getSize().x/2.0f, circleTexture.getSize().y/2.0f);

	blackRect.setFillColor(sf::Color());
	blackRect.setPosition(0, 0);

	font.loadFromFile("assets/fonts/LiberationMono-Regular.ttf");
}

void LightOverlay::Update(const Level& level, const sf::View& view)
{
	overlayTexture.clear(sf::Color(0,0,0,255));
	circleOverlay.clear(sf::Color(0,0,0,255));

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


	sf::Vector2f viewcorrection(viewPos.x - viewSize.x/2, viewPos.y - viewSize.y/2);

	for(int i = 0; i <= rays; i++) {
		/*circles.push_back(sf::CircleShape(2.0f));
		circles.back().setPosition(point1);
		std::stringstream sstream;
		sstream << angle1;
		texts.push_back(sf::Text(sf::String(sstream.str()), font, 12));
		texts.back().setPosition(point1);*/

		sf::Vector2f relPoint1 = point1 - playerPos;
		sf::Vector2f relPoint2 = point2 - playerPos;

		//Allow the points to penetrate the walls a little bit
		/*if(abs(relPoint1.x) > abs(relPoint1.y)) {
			relPoint1.x += relPoint1.x/abs(relPoint1.x) * 16;
		} else {
			relPoint1.y += relPoint1.y/abs(relPoint1.y) * 16;
		}

		if(abs(relPoint2.x) > abs(relPoint2.y)) {
			relPoint2.x += relPoint2.x/abs(relPoint2.x) * 16;
		} else {
			relPoint2.y += relPoint2.y/abs(relPoint2.y) * 16;
		}*/

		sf::ConvexShape triangle;
		triangle.setPointCount(3);
		triangle.setPoint(0, sf::Vector2f(0.0f, 0.0f));
		triangle.setPoint(1, sf::Vector2f(relPoint1.x, -relPoint1.y));
		triangle.setPoint(2, sf::Vector2f(relPoint2.x, -relPoint2.y));
		triangle.setFillColor(sf::Color(0, 0, 0, 0));
		triangle.setPosition(sf::Vector2f(playerPos.x - viewcorrection.x, viewSize.y-(playerPos.y - viewcorrection.y)));
		overlayTexture.draw(triangle, sf::BlendMode::BlendNone);

		found1 = found2;
		point1 = point2;
		angle1 = angle2;
		angle2 = 2.0f*3.14159/(float)rays * (i+1);
		found2 = level.GetCollide(level.GetPlayer().GetPos(), angle2, point2);
	}

	circleSprite.setPosition(sf::Vector2f(playerPos.x - viewcorrection.x, playerPos.y - viewcorrection.y));
	//circleOverlay.draw(blackRect);
	circleOverlay.draw(circleSprite, sf::BlendMode::BlendMultiply);

	overlayTexture.draw(circleOverlaySprite, sf::BlendMode::BlendAdd);
	overlaySprite.setPosition(view.getCenter());
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