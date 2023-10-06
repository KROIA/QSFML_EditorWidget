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
#ifdef QSFML_OPTIMIZED
        float sqrL = vec.x * vec.x + vec.y * vec.y;

        if (sqrL == 0)
            return 0;

        float angle = atan2(vec.y, vec.x);
        angle = fmod(angle + M_PI, 2 * M_PI);
        
        return (angle < 0) ? angle + 2 * M_PI - M_PI : angle - M_PI;
#else

        float sqrL = getSquareLength(vec);
        if(sqrL <= 0)
            return 0;
        float angle;
        sqrL = sqrt(sqrL);

        if (vec.y <= 0) {
            angle = -acos(vec.x / sqrL);
        }
        else {
            angle = acos(vec.x / sqrL);
        }
        return getNormalzedAngle(angle);
#endif
        return 0;
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

        return getNormalzedAngle(angle);
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

    float getNormalzedAngle(float angle)
    {
#ifdef QSFML_OPTIMIZED
        angle = fmod(angle + M_PI, 2 * M_PI);
        return (angle < 0) ? angle + 2 * M_PI - M_PI : angle - M_PI;
#else
        angle = fmod(angle + M_PI, 2 * M_PI);
        if (angle < 0)
            angle += 2 * M_PI;
        return angle - M_PI;
#endif
    }
    bool isAngleInRange(float angle, float minAngle, float maxAngle)
    {
        // Normalize angles to be in the range [0, 2*pi)
        angle = fmod(angle, M_2PI);
        if (angle < 0)
            angle += M_2PI;

        minAngle = fmod(minAngle, M_2PI);
        if (minAngle < 0)
            minAngle += M_2PI;

        maxAngle = fmod(maxAngle, M_2PI);
        if (maxAngle < 0)
            maxAngle += M_2PI;

        // Case where the angle wraps around
        if (minAngle > maxAngle) {
            return (angle >= minAngle || angle <= maxAngle);
        }

        // Case where the angle does not wrap around
        return (angle >= minAngle && angle <= maxAngle);
    }
}
}
