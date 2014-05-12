
#include "LightSource.h"
#include "Level.h"

#include <sstream>

sf::Texture LightSource::circleTexture;
sf::Font LightSource::font;
bool LightSource::fontLoaded = false;

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

	if(!fontLoaded) {
		font.loadFromFile("assets/font/monkey.ttf");
		fontLoaded = true;
	}
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
		circles.clear();
		texts.clear();

		const std::vector<Level::Corner> &corners = level.GetCorners();
		std::list<float> angles;
		std::list<sf::Vector2f> positions;
		std::list<Level::Corner::FacingType> types;
		//If the light's position has changed, we need to update the position of the light
		compositeOverlaySprite.setPosition(lightPos);

		//First, clear the visibility texture entirely with black
		triangleOverlay.clear();

		//Get things in polar coords and sort by angle
		for(unsigned i = 0; i < corners.size(); i++) {
			sf::Vector2f extendPoint;
			Level::Corner corner = corners[i];
			Level::Corner::FacingType facing = corner.getFacingType(lightPos);
			bool insert = false;
			//Just cull any walls that are facing away
			if(facing == Level::Corner::FACING_AWAY) {
				continue;
			}

			//Get the angle of the corner from this light
			sf::Vector2f cornerRel = corner.pos - lightPos;
			float angle = atan2(cornerRel.y, cornerRel.x);

			//Figure out if the corner is visible
			sf::Vector2f point;
			bool found = level.GetCollide(lightPos, cornerRel, point);

			float cornerMag = magnitude(cornerRel);
			float collideMag = magnitude(point - lightPos);
			/*if(facing == Level::Corner::FACING_CORNER) {
				//Just check if the place we found is pretty close
				// to the actual position of the corner
				if(collideMag + 8.0f >= cornerMag) {
					insert = true;
				}
			} else if(facing == Level::Corner::FACING_TANGENT_FIRST ||
					facing == Level::Corner::FACING_TANGENT_SECOND) {
				//In this case, we're going to plan that it might
				// have extended past the corner
				if(collideMag + 8.0f >= cornerMag) {
					//Probably an extend
					extendPoint = point;
					insert = true;
				} else if(magnitude(point - corner.pos) < 8.0) {
					//Probably hit the corner, we got to keep going
					point += sf::Vector2f(8.0f * cos(angle), 8.0f * sin(angle));
					bool foundExtend = level.GetCollide(point, angle, extendPoint);
					insert = true;
				}
			}*/
			
			auto iter = angles.begin();
			auto positionIter = positions.begin();
			auto typeIter = types.begin();
			while(iter != angles.end() && *iter < angle) {
				++iter;
				++positionIter;
				++typeIter;
			}

			insert = true;
			if(insert) {
				if(facing == Level::Corner::FACING_TANGENT_FIRST) {
					angles.insert(iter, angle);
					positions.insert(positionIter, extendPoint);
					types.insert(typeIter, facing);
				}
				if(facing == Level::Corner::FACING_TANGENT_FIRST ||
						facing == Level::Corner::FACING_TANGENT_SECOND) {
					angles.insert(iter, angle);
					positions.insert(positionIter, corner.pos);
					types.insert(typeIter, facing);
				} else {
					angles.insert(iter, angle);
					positions.insert(positionIter, point);
					types.insert(typeIter, facing);
				}
				if(facing == Level::Corner::FACING_TANGENT_SECOND) {
					angles.insert(iter, angle);
					positions.insert(positionIter, extendPoint);
					types.insert(typeIter, facing);
				}
			}
		}

		auto point1 = positions.begin();
	    auto point2 = std::next(point1);
		auto type1 = types.begin();
	    auto type2 = std::next(type1);

		const float debugRad = 2.0;

		/*for(int i = 0; i < corners.size(); i++) {
			sf::CircleShape shape(debugRad);
			shape.setOrigin(debugRad/2.0f, debugRad/2.0f);
			shape.setPosition(corners[i].pos);
			shape.setFillColor(sf::Color::White);
			circles.push_back(shape);

			std::stringstream sstream;
			Level::Corner::FacingType facing = corners[i].getFacingType(lightPos);
			if(facing == Level::Corner::FACING_AWAY) {
				sstream << "AWAY";
			} else if(facing == Level::Corner::FACING_CORNER) {
				sstream << "CORNER";
			} else if(facing == Level::Corner::FACING_TANGENT_FIRST) {
				sstream << "T_FIRST";
			} else if(facing == Level::Corner::FACING_TANGENT_SECOND) {
				sstream << "T_SECOND";
			}
			
			sstream << corners[i].quadrant << " " << getQuadrant(corners[i].pos, lightPos) << " " << corners[i].inner;

			sf::Text text(sstream.str(), font, 10);
			text.setPosition(corners[i].pos);
			text.setColor(sf::Color::White);
			texts.push_back(text);
		}*/
			sf::CircleShape shape(debugRad);
			shape.setOrigin(debugRad/2.0f, debugRad/2.0f);
			shape.setPosition(*point1);
			shape.setFillColor(sf::Color::White);
			circles.push_back(shape);

			std::stringstream sstream;
			if(*type1 == Level::Corner::FACING_AWAY) {
				sstream << "AWAY";
			} else if(*type1 == Level::Corner::FACING_CORNER) {
				sstream << "CORNER";
			} else if(*type1 == Level::Corner::FACING_TANGENT_FIRST) {
				sstream << "T_FIRST";
			} else if(*type1 == Level::Corner::FACING_TANGENT_SECOND) {
				sstream << "T_SECOND";
			}

			sf::Text text(sstream.str(), font, 10);
			text.setPosition(*point1);
			text.setColor(sf::Color::White);
			texts.push_back(text);

		//Loop through the number of rays we want to shoot out
		for(unsigned i = 1; i <= positions.size(); i++) {
			sf::CircleShape shape(debugRad);
			shape.setOrigin(debugRad/2.0f, debugRad/2.0f);
			shape.setPosition(*point2);
			shape.setFillColor(sf::Color::White);
			circles.push_back(shape);

			std::stringstream sstream;
			if(*type2 == Level::Corner::FACING_AWAY) {
				sstream << "AWAY";
			} else if(*type2 == Level::Corner::FACING_CORNER) {
				sstream << "CORNER";
			} else if(*type2 == Level::Corner::FACING_TANGENT_FIRST) {
				sstream << "T_FIRST";
			} else if(*type2 == Level::Corner::FACING_TANGENT_SECOND) {
				sstream << "T_SECOND";
			}

			sf::Text text(sstream.str(), font, 10);
			text.setPosition(*point2);
			text.setColor(sf::Color::White);
			texts.push_back(text);

			//Get the relative position of the points we intersected at
			sf::Vector2f relPoint1 = *point1 - lightPos;
			sf::Vector2f relPoint2 = *point2 - lightPos;
			float mag1 = magnitude(relPoint1);
			float mag2 = magnitude(relPoint2);

			//Allow the points to penetrate the walls a little bit
			/*relPoint1 = relPoint1 * (mag1+8.0f)/mag1;
			relPoint2 = relPoint2 * (mag2+8.0f)/mag2;*/
			
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
			type1 = type2++;
			if(++point2 == positions.end()) {
				point2 = positions.begin();
				type2 = types.begin();
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

void LightSource::debugDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
	for(unsigned i = 0; i < circles.size(); i++) {
		target.draw(circles[i], state);
	}
	for(unsigned i = 0; i < texts.size(); i++) {
		target.draw(texts[i], state);
	}
}

bool LightSource::GetIsOn()
{
	return isOn;
}
