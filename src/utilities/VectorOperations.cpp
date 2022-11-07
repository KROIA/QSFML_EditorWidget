#include "utilities/VectorOperations.h"
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

    sf::Vector2f getRotated(const sf::Vector2f &vec, float rad)
    {
        float angle = getAngle(vec);
        angle -= rad;
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
        if(vec.x > 0)
        {
            angle = asin(vec.y / sqrL);
        }
        else
        {
            angle = M_PI - asin(vec.y / sqrL);
        }
        return angle;
    }
    float getAngle(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        float l1 = getLength(vec1);
        float l2 = getLength(vec2);
        float prod = l1 * l2;
        if(prod == 0)
            return 0;

        return acos(dotProduct(vec1, vec2) / (l1 *l2));
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
}
}
