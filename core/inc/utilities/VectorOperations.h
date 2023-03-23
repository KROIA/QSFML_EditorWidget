#pragma once

#include "SFML/Graphics.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace QSFML
{
namespace VectorMath
{
    extern float getLength(const sf::Vector2f &vec);
    extern float getSquareLength(const sf::Vector2f &vec);
    extern sf::Vector2f getRotated(const sf::Vector2f &vec, float rad);
    extern sf::Vector2f getRotated(const sf::Vector2f &vec, const sf::Vector2f &origin, float rad);
    extern float getAngle(const sf::Vector2f &vec);
    extern float getAngle(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    extern float dotProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    extern float crossProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    extern const sf::Vector2f &getUnitVector();
    extern sf::Vector2f getNormalized(const sf::Vector2f &vec);
}
}
