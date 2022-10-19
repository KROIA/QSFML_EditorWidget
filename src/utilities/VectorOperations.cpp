#include "VectorOperations.h"
#include <math.h>

float getVectorLength(const sf::Vector2f &vec)
{
    return sqrt( vec.x * vec.x + vec.y * vec.y);
}
