#include "utilities/VectorOperations.h"
#include "utilities/RandomEngine.h"
#include <math.h>

namespace QSFML
{
namespace VectorMath
{
    float getLength(const sf::Vector2f &vec)
    {
        return sqrt( vec.x * vec.x + vec.y * vec.y);
    }
    float getSquareLength(const sf::Vector2f &vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }
    sf::Vector2f getRotatedUnitVector(float rad)
    {
        return sf::Vector2f(cos(rad), sin(rad));
    }
    sf::Vector2f getRotated(const sf::Vector2f &vec, float rad)
    {
        float angle = getAngle(vec);
        angle += rad;
        return sf::Vector2f(cos(angle), sin(angle)) * getLength(vec);
    }
    sf::Vector2f getRotated(const sf::Vector2f &vec, const sf::Vector2f &origin, float rad)
    {
        return getRotated(vec - origin, rad) + origin;
    }
    float getAngle(const sf::Vector2f &vec)
    {
        float sqrL = getSquareLength(vec);
        if(sqrL <= 0)
            return 0;
        float angle;
        sqrL = sqrt(sqrL);

        if (vec.y >= 0) {
            angle = acos(vec.x / sqrL);
        }
        else {
            angle = -acos(vec.x / sqrL);
        }
        return angle;
    }
    float getAngle(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        float dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;
        float determinant = vec1.x * vec2.y - vec1.y * vec2.x;
        float length = getLength(vec1) * getLength(vec2);

        float angle = 0.0f;

        if (determinant >= 0) {
            angle = acos(dotProduct/ length);
        }
        else {
            angle = 2 * M_PI - acos(dotProduct/ length);
        }

        return angle;
    }
    float dotProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }
    float crossProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        return vec1.x * vec2.y - vec1.y * vec2.x;
    }
    const sf::Vector2f &getUnitVector()
    {
        static const sf::Vector2f v(1,0);
        return v;
    }
    sf::Vector2f getNormalized(const sf::Vector2f &vec)
    {
        float l = vec.x * vec.x + vec.y * vec.y;
        if(l <= 0)
            return vec;
        return(vec / (float)sqrt(l));
    }
}
}
