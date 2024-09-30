#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "QSFML_EditorWidget_base.h"


namespace QSFML
{
namespace VectorMath
{
    typedef sf::Vector2<double> Vector2d;
    QSFML_EDITOR_WIDGET_EXPORT extern float getLength(const sf::Vector2f &vec);
    QSFML_EDITOR_WIDGET_EXPORT extern double getLength(const Vector2d&vec);
    QSFML_EDITOR_WIDGET_EXPORT extern float getSquareLength(const sf::Vector2f &vec);
    QSFML_EDITOR_WIDGET_EXPORT extern double getSquareLength(const Vector2d&vec);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotatedUnitVector(float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern Vector2d getRotatedUnitVector(double rad);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotated(const sf::Vector2f &vec, float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern Vector2d getRotated(const Vector2d&vec, double rad);
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getRotated(const sf::Vector2f &vec, const sf::Vector2f &origin, float rad);
    QSFML_EDITOR_WIDGET_EXPORT extern Vector2d getRotated(const Vector2d &vec, const Vector2d&origin, double rad);
    QSFML_EDITOR_WIDGET_EXPORT extern float getAngle(const sf::Vector2f& vec);
    QSFML_EDITOR_WIDGET_EXPORT extern double getAngle(const Vector2d& vec);
    

    /*
        Returns the counterclockwise angle in radiant between vec1 and vec2.
        The angle points from vec1 to vec2. If the direction of that angle is clockwise, the angle is negative.
    */
    QSFML_EDITOR_WIDGET_EXPORT extern float getAngle(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern double getAngle(const Vector2d &vec1, const Vector2d &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern float dotProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern double dotProduct(const Vector2d &vec1, const Vector2d &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern float crossProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern double crossProduct(const Vector2d &vec1, const Vector2d &vec2);
    QSFML_EDITOR_WIDGET_EXPORT extern const sf::Vector2f &getUnitVector();
    QSFML_EDITOR_WIDGET_EXPORT extern const Vector2d &getUnitVectorD();
    QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f getNormalized(const sf::Vector2f &vec);
    QSFML_EDITOR_WIDGET_EXPORT extern Vector2d getNormalized(const Vector2d &vec);

    QSFML_EDITOR_WIDGET_EXPORT extern float getNormalzedAngle(float angle);
    QSFML_EDITOR_WIDGET_EXPORT extern double getNormalzedAngle(double angle);
    QSFML_EDITOR_WIDGET_EXPORT extern bool isAngleInRange(float angle, float minAngle, float maxAngle);
    QSFML_EDITOR_WIDGET_EXPORT extern bool isAngleInRange(double angle, double minAngle, double maxAngle);


	QSFML_EDITOR_WIDGET_EXPORT extern sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
	QSFML_EDITOR_WIDGET_EXPORT extern Vector2d lerp(const Vector2d& start, const Vector2d& end, double t);


}

}
