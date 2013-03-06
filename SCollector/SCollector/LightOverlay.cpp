
#include "LightOverlay.h"
#include "Level.h"

LightOverlay::LightOverlay(int rays, int radius, const sf::Vector2f& pos, const sf::View& view)
{
	//Initialize attributes
	this->rays = rays;
	this->radius = radius;
	this->lightPos = pos;
	needsUpdate = true;

	//Create textures
	compositeOverlay.create(radius*2, radius*2);
	compositeOverlaySprite.setTexture(compositeOverlay.getTexture());
	compositeOverlaySprite.setOrigin(radius, radius);

	triangleOverlay.create(radius*2, radius*2);
	triangleOverlaySprite.setTexture(triangleOverlay.getTexture());
	triangleOverlaySprite.setOrigin(radius, radius);
	triangleOverlaySprite.setPosition(radius, radius);
	
	//Load the light circle image
	circleImage.loadFromFile("assets/img/LightAura.png");
	circleTexture.loadFromImage(circleImage);
	circleSprite.setTexture(circleTexture);
	circleSprite.setScale(sf::Vector2f(radius*2.0f/circleTexture.getSize().x,
							radius*2.0f/circleTexture.getSize().y));
	circleSprite.setOrigin(circleTexture.getSize().x/2.0f, circleTexture.getSize().y/2.0f);
	circleSprite.setPosition(radius, radius);
}

void LightOverlay::SetPos(const sf::Vector2f& pos)
{
	//Only say we need an update if the new position is not the same as the old
	if(lightPos != pos)
	{
		lightPos = pos;
		needsUpdate = true;
	}
}

void LightOverlay::Update(const Level& level, const sf::View& view)
{
	//Get some information about the view
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f viewPos = view.getCenter();

	//Since we're drawing the light texture relative to the view, we need to
	// correct its position relative to the view rectangle
	sf::Vector2f viewcorrection(viewPos.x - viewSize.x/2, viewPos.y - viewSize.y/2);

	//If the view position has changed, we need to update the position of the light
	compositeOverlaySprite.setPosition(sf::Vector2f(lightPos.x - viewcorrection.x,
										lightPos.y - viewcorrection.y));

	//Recompute the visibility texture if we need to
	if(needsUpdate) {
		//First, clear the visibility texture entirely with black
		triangleOverlay.clear(sf::Color(0,0,0,255));

		//Initialize the beginning parameters
		//Basically, what we do here is shoot out lines defined by angles
		// (rather than slopes) crossing through the point defined by the light's
		// position
		float angle1 = 0;
		float angle2 = 2*3.14159/(float)rays;
		sf::Vector2f point1, point2;
		bool found1, found2;

		//Get the first two points at the angles
		found1 = level.GetCollide(lightPos, angle1, point1);
		found2 = level.GetCollide(lightPos, angle2, point2);

		//Loop through the number of rays we want to shoot out
		for(int i = 0; i <= rays; i++) {
			//Get the relative position of the points we intersected at
			sf::Vector2f relPoint1 = point1 - lightPos;
			sf::Vector2f relPoint2 = point2 - lightPos;
			float mag1 = sqrt(relPoint1.x*relPoint1.x + relPoint1.y*relPoint1.y);
			float mag2 = sqrt(relPoint2.x*relPoint2.x + relPoint2.y*relPoint2.y);

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
			triangle.setPosition(radius, radius);
			triangleOverlay.draw(triangle, sf::BlendNone);

			//Get the next point, preserving the current one for the next triangle
			found1 = found2;
			point1 = point2;
			angle1 = angle2;
			angle2 = 2.0f*3.14159/(float)rays * (i+1);
			found2 = level.GetCollide(lightPos, angle2, point2);
		}

		//Draw our changes to the texture
		triangleOverlay.display();
		triangleOverlaySprite.setTexture(triangleOverlay.getTexture());

		//Composite the visibility texture and the circle aura texture together
		compositeOverlay.clear();
		compositeOverlay.draw(circleSprite, sf::BlendMode::BlendMultiply);
		compositeOverlay.draw(triangleOverlaySprite, sf::BlendMode::BlendAdd);
		compositeOverlay.display();
		compositeOverlaySprite.setTexture(compositeOverlay.getTexture());

		//We no longer need an update until the next time the light changes position
		needsUpdate = false;
	}
}

void LightOverlay::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(compositeOverlaySprite, state);
}