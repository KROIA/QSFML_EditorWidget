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
}
}
