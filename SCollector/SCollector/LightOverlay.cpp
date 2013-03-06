
#include "LightOverlay.h"
#include "Level.h"

LightOverlay::LightOverlay(int rays, int radius, const sf::Vector2f& pos, const sf::View& view)
{
	this->rays = rays;
	this->radius = radius;
	this->lightPos = pos;
	needsUpdate = true;

	compositeOverlay.create(radius*2, radius*2);
	compositeOverlaySprite.setTexture(compositeOverlay.getTexture());
	compositeOverlaySprite.setOrigin(radius, radius);

	triangleOverlay.create(radius*2, radius*2);
	triangleOverlaySprite.setTexture(triangleOverlay.getTexture());
	triangleOverlaySprite.setOrigin(radius, radius);
	
	circleImage.loadFromFile("assets/img/LightAura.png");
	circleTexture.loadFromImage(circleImage);
	circleSprite.setTexture(circleTexture);
	circleSprite.setScale(sf::Vector2f(radius*2.0f/circleTexture.getSize().x,
							radius*2.0f/circleTexture.getSize().y));
	circleSprite.setOrigin(circleTexture.getSize().x/2.0f, circleTexture.getSize().y/2.0f);

	font.loadFromFile("assets/fonts/LiberationMono-Regular.ttf");
}

void LightOverlay::SetPos(const sf::Vector2f& pos)
{
	lightPos = pos;
	needsUpdate = true;
}

void LightOverlay::Update(const Level& level, const sf::View& view)
{
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f viewPos = view.getCenter();

	//Correction for the view
	sf::Vector2f viewcorrection(viewPos.x - viewSize.x/2, viewPos.y - viewSize.y/2);

	//This recomputes the lighting if need be
	if(needsUpdate) {
		triangleOverlay.clear(sf::Color(0,0,0,255));

		circles.clear();
		texts.clear();

		float angle1 = 0;
		float angle2 = 2*3.14159/(float)rays;
		sf::Vector2f point1, point2;
		bool found1, found2;
		float minDist;
		sf::Vector2u imgSize = circleImage.getSize();
		found1 = level.GetCollide(level.GetPlayer().GetPos(), angle1, point1);
		found2 = level.GetCollide(level.GetPlayer().GetPos(), angle2, point2);

		for(int i = 0; i <= rays; i++) {
			sf::Vector2f relPoint1 = point1 - lightPos;
			sf::Vector2f relPoint2 = point2 - lightPos;

			//Debug code to show where the lines are intersecting the walls and
			// at what angles
			/*circles.push_back(sf::CircleShape(2.0f));
			circles.back().setPosition(sf::Vector2f(relPoint1.x+radius, -relPoint2.y+radius));
			std::stringstream sstream;
			sstream << angle1;
			texts.push_back(sf::Text(sf::String(sstream.str()), font, 12));
			texts.back().setPosition(relPoint1);*/

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
			triangle.setPoint(1, sf::Vector2f(relPoint1.x, relPoint1.y));
			triangle.setPoint(2, sf::Vector2f(relPoint2.x, relPoint2.y));
			triangle.setFillColor(sf::Color(0, 0, 0, 0));
			triangle.setPosition(radius, radius);
			triangleOverlay.draw(triangle, sf::BlendNone);

			found1 = found2;
			point1 = point2;
			angle1 = angle2;
			angle2 = 2.0f*3.14159/(float)rays * (i+1);
			found2 = level.GetCollide(level.GetPlayer().GetPos(), angle2, point2);
		}

		triangleOverlay.display();
		triangleOverlaySprite.setTexture(triangleOverlay.getTexture());
		needsUpdate = false;
	}

	circleSprite.setPosition(radius, radius);
	triangleOverlaySprite.setPosition(radius, radius);

	compositeOverlay.clear();
	compositeOverlay.draw(circleSprite, sf::BlendMode::BlendMultiply);
	compositeOverlay.draw(triangleOverlaySprite, sf::BlendMode::BlendAdd);
	for(int i = 0; i < circles.size(); i++)
		compositeOverlay.draw(circles[i]);
	compositeOverlay.display();
	compositeOverlaySprite.setTexture(compositeOverlay.getTexture());
	compositeOverlaySprite.setPosition(sf::Vector2f(lightPos.x - viewcorrection.x, lightPos.y - viewcorrection.y));
}

void LightOverlay::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(compositeOverlaySprite, state);
}