
#include "LightSource.h"
#include "Level.h"

#include <sstream>

sf::Texture LightSource::circleTexture;
#if DEBUGDRAW
sf::Font LightSource::font;
bool LightSource::fontLoaded = false;
#endif

LightSource::LightSource(int radius, const sf::Color &color, const sf::Vector2f &pos) :
	circleOverlay((float)radius, 32)
{
	this->radius = radius;
	this->lightPos = pos;
	needsUpdate = true;
	needsRedraw = true;
	isOn = false;

	//Create textures
	compositeOverlay.create(radius*2, radius*2);
	compositeOverlaySprite.setTexture(compositeOverlay.getTexture());
	compositeOverlaySprite.setOrigin((float)radius, (float)radius);

	triangleOverlay.create(radius*2, radius*2);
	triangleOverlaySprite.setTexture(triangleOverlay.getTexture());
	triangleOverlaySprite.setOrigin((float)radius, (float)radius);
	triangleOverlaySprite.setPosition((float)radius, (float)radius);
	
	//Load the light circle image, if it hasn't already
	if(circleTexture.getSize().x <= 0 || circleTexture.getSize().y <= 0) {
		circleTexture.loadFromFile("assets/img/LightAura.png");
	}
	circleSprite.setTexture(circleTexture);
	circleSprite.setScale(sf::Vector2f(radius*2.0f/circleTexture.getSize().x,
							radius*2.0f/circleTexture.getSize().y));

	//Create the light/intensity overlay
	circleOverlay.setFillColor(color);

#if DEBUGDRAW
	if(!fontLoaded) {
		font.loadFromFile("assets/font/monkey.ttf");
		fontLoaded = true;
	}
#endif
}

void LightSource::SetPos(const sf::Vector2f& pos)
{
	//Only say we need an update if the new position is not the same as the old
	if(lightPos != pos)
	{
		lightPos = pos;
		needsUpdate = true;
	}
}

void LightSource::ForceUpdate()
{
	needsUpdate = true;
}

void LightSource::SetColor(const sf::Color& color)
{
	circleOverlay.setFillColor(color);
	needsRedraw = true;
}

void LightSource::Toggle()
{
	isOn = !isOn;
}

void LightSource::Update(const Level& level)
{
	//Recompute the visibility texture only if we need to
	if(isOn && needsUpdate) {
#if DEBUGDRAW
		circles.clear();
		texts.clear();
#endif

		const std::vector<Level::LightPoint> &unsortedPoints = level.IntersectWalls(lightPos);
		std::list<float> angles;
		std::list<Level::LightPoint> points;

		//Sort the points by angle ascending; we assume that the fillTo and fillFrom
		// properties have the same relative angle from lightPos
		for(std::size_t i = 0; i < unsortedPoints.size(); i++) {
			Level::LightPoint point = unsortedPoints[i];
			sf::Vector2f relVec = point.fillTo - lightPos;
			float mag = magnitude(relVec);
			float angle = atan2f(relVec.y, relVec.x);

			auto angleIter = angles.begin();
			auto pointIter = points.begin();
			while(angleIter != angles.end() && *angleIter < angle) {
				++angleIter;
				++pointIter;
			}
			angles.insert(angleIter, angle);
			points.insert(pointIter, point);
			
#if DEBUGDRAW
			sf::CircleShape shape(2.0f);
			shape.setOrigin(1.0f, 1.0f);
			shape.setPosition(point.fillFrom);
			shape.setFillColor(sf::Color::White);
			circles.push_back(shape);

			std::stringstream sstream;
			sstream << (point.fillFrom.x - lightPos.x) << "\n" << (point.fillFrom.y - lightPos.y);

			sf::Text text(sstream.str(), font, 10);
			text.setPosition(point.fillFrom);
			text.setColor(sf::Color::White);
			texts.push_back(text);

			if(point.fillFrom != point.fillTo) {
				sf::CircleShape shape(2.0f);
				shape.setOrigin(1.0f, 1.0f);
				shape.setPosition(point.fillTo);
				shape.setFillColor(sf::Color::White);
				circles.push_back(shape);

				std::stringstream sstream;
				sstream << (point.fillTo.x - lightPos.x) << "\n" << (point.fillTo.y - lightPos.y);

				sf::Text text(sstream.str(), font, 10);
				text.setPosition(point.fillTo);
				text.setColor(sf::Color::White);
				texts.push_back(text);
			}
#endif
		}

		//If the light's position has changed, we need to update the position of the light
		compositeOverlaySprite.setPosition(lightPos);

		//First, clear the visibility texture entirely with black
		triangleOverlay.clear();

		auto point1 = points.begin();
	    auto point2 = std::next(point1);

		const float debugRad = 2.0;

		//Loop through the number of rays we want to shoot out
		for(unsigned i = 1; i <= points.size(); i++) {
			sf::Vector2f p1 = point1->fillFrom;
			sf::Vector2f p2 = point2->fillTo;
			//Get the relative position of the points we intersected at
			sf::Vector2f relPoint1 = p1 - lightPos;
			sf::Vector2f relPoint2 = p2 - lightPos;
			float mag1 = magnitude(relPoint1);
			float mag2 = magnitude(relPoint2);

			//Allow the points to penetrate the walls a little bit
			relPoint1 = relPoint1 * (mag1+8.0f)/mag1;
			relPoint2 = relPoint2 * (mag2+8.0f)/mag2;
			
			//Create a triangle using the light's position and the two points
			sf::ConvexShape triangle;
			triangle.setPointCount(3);
			triangle.setPoint(0, sf::Vector2f(0.0f, 0.0f));
			triangle.setPoint(1, sf::Vector2f(relPoint1.x, relPoint1.y));
			triangle.setPoint(2, sf::Vector2f(relPoint2.x, relPoint2.y));
			triangle.setFillColor(sf::Color(0, 0, 0, 0));
			//The triangle's origin is (0, 0), where the light source is located, and
			// (radius, radius) is where the light source is located in the texture
			triangle.setPosition((float)radius, (float)radius);
			triangleOverlay.draw(triangle, sf::BlendNone);

			//Get the next point, preserving the current one for the next triangle
			point1 = point2;
			if(++point2 == points.end()) {
				point2 = points.begin();
			}
		}

		//Draw our changes to the texture
		triangleOverlay.display();
		triangleOverlaySprite.setTexture(triangleOverlay.getTexture());

		//We no longer need an update until the next time the light changes position
		needsRedraw = true;
		needsUpdate = false;
	}

	if(needsRedraw) {
		//Composite the visibility texture, the circle aura texture and the color/dimness
		// overlay together
		compositeOverlay.clear();
		compositeOverlay.draw(circleSprite, sf::BlendMultiply);
		compositeOverlay.draw(circleOverlay, sf::BlendAlpha);
		compositeOverlay.draw(triangleOverlaySprite, sf::BlendAdd);
		compositeOverlay.display();
		compositeOverlaySprite.setTexture(compositeOverlay.getTexture());
		needsRedraw = false;
	}
}

void LightSource::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	if(isOn)
		target.draw(compositeOverlaySprite, state);
}

void LightSource::debugDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
#if DEBUGDRAW
	for(unsigned i = 0; i < circles.size(); i++) {
		target.draw(circles[i], state);
	}
	for(unsigned i = 0; i < texts.size(); i++) {
		target.draw(texts[i], state);
	}
#endif
}

bool LightSource::GetIsOn()
{
	return isOn;
}
