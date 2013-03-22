/**
 * \brief Collection of utility functions
 * \author Edward Lu
 * \date March 20 2013
 */

#ifndef __UTILITY_H
#define __UTILITY_H

#include <SFML/System.hpp>

#include <random>

#define PI 3.14159f
#define TO_DEG 57.2958279f
#define TO_RAD 0.0174532778f

/** Gets the magnitude of vec */
float magnitude(const sf::Vector2f& vec);
/** Shifts the angle to between 0 and 2*PI */
float shiftAngle(float angle);
/** Initializes the random seed engine that getRandom() uses. */
void initRandom();
/** Gets a random number in the range [0, 1]. */
float getRandom();
/** Gets a random number in the range [begin, end]. */
float getRandom(float begin, float end);

#endif