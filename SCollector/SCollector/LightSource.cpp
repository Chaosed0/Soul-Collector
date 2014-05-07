
#include "LightSource.h"
#include "Level.h"

sf::Texture LightSource::circleTexture;

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
	//Recompute the visibility texture if we need to
	if(needsUpdate) {

		const std::vector<Level::Corner> &corners = level.GetCorners();
		std::list<float> angles;
		std::list<unsigned> indices;
		//If the light's position has changed, we need to update the position of the light
		compositeOverlaySprite.setPosition(lightPos);

		//First, clear the visibility texture entirely with black
		triangleOverlay.clear();

		//Get things in polar coords and sort by angle
		for(unsigned i = 0; i < corners.size(); i++) {
			sf::Vector2f rel = corners[i].pos - lightPos;
			float angle = atan2(rel.y, rel.x);
			auto iter = angles.begin();
			while(iter != angles.end() && *iter < angle) {
				++iter;
			}
			angles.insert(iter, angle);
		}

		//Initialize the beginning parameters
		//Basically, what we do here is shoot out lines defined by angles
		// (rather than slopes) crossing through the point defined by the light's
		// position
		auto angle1 = angles.begin();
		auto angle2 = std::next(angle1);
		sf::Vector2f point1, point2;
		bool found1, found2;

		//Get the first two points at the angles
		found1 = level.GetCollide(lightPos, *angle1, point1);
		found2 = level.GetCollide(lightPos, *angle2, point2);

		//Loop through the number of rays we want to shoot out
		for(unsigned i = 1; i <= angles.size(); i++) {
			//Get the relative position of the points we intersected at
			sf::Vector2f relPoint1 = point1 - lightPos;
			sf::Vector2f relPoint2 = point2 - lightPos;
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
			found1 = found2;
			point1 = point2;
			angle1 = angle2;
			if(++angle2 == angles.end()) {
				angle2 = angles.begin();
			}
			found2 = level.GetCollide(lightPos, *angle2, point2);
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
		compositeOverlay.draw(circleSprite, sf::BlendMode::BlendMultiply);
		compositeOverlay.draw(circleOverlay, sf::BlendMode::BlendAlpha);
		compositeOverlay.draw(triangleOverlaySprite, sf::BlendMode::BlendAdd);
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

bool LightSource::GetIsOn()
{
	return isOn;
}
