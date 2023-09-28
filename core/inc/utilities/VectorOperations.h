#pragma once

#include "QSFML_base.h"

namespace QSFML
{
namespace VectorMath
{
    QSFML_EDITOR_WIDGET_EXPORT extern float getLength(const sf::Vector2f &vec);
    QSFML_EDITOR_WIDGET_EXPORT extern float getSquareLength(const sf::Vector2f &vec);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotatedUnitVector(float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotated(const sf::Vector2f &vec, float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotated(const sf::Vector2f &vec, const sf::Vector2f &origin, float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern float getAngle(const sf::Vector2f &vec);

    /*
        Returns the counterclockwise angle in radiant between vec1 and vec2.
        The angle points from vec1 to vec2. If the direction of that angle is clockwise, the angle is negative.
    */
    QSFML_EDITOR_WIDGET_EXPORT extern float getAngle(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern float dotProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern float crossProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern const sf::Vector2f &getUnitVector();
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getNormalized(const sf::Vector2f &vec);
}
}
