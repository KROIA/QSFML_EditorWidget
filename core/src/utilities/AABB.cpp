#include "utilities/AABB.h"
#include <limits>

namespace QSFML
{
namespace Utilities
{

AABB AABB::getFrame(const std::vector<AABB> &list)
{
    sf::Vector2f min, max;
    min.x = std::numeric_limits<float>::max();
    min.y = std::numeric_limits<float>::max();

    max.x = std::numeric_limits<float>::min();
    max.y = std::numeric_limits<float>::min();
    for(size_t i=0; i<list.size(); i++)
    {
        sf::Vector2f pos = list[i].m_pos;
        sf::Vector2f size = list[i].m_size;
        if(min.x > pos.x)
            min.x = pos.x;
        if(min.y > pos.y)
            min.y = pos.y;

        if(max.x < size.x + pos.x)
            max.x = size.x + pos.x;
        if(max.y < size.y + pos.y)
            max.y = size.y + pos.y;
    }
    return AABB(min,max - min);
}
AABB AABB::getFrame(const std::vector<sf::Vector2f>& points)
{
    sf::Vector2f min, max;
    min.x = std::numeric_limits<float>::max();
    min.y = std::numeric_limits<float>::max();

    max.x = std::numeric_limits<float>::min();
    max.y = std::numeric_limits<float>::min();
    for (size_t i = 0; i < points.size(); i++)
    {
        sf::Vector2f pos = points[i];
        if (min.x > pos.x)
            min.x = pos.x;
        if (min.y > pos.y)
            min.y = pos.y;

        if (max.x < pos.x)
            max.x = pos.x;
        if (max.y < pos.y)
            max.y = pos.y;
    }
    return AABB(min, max - min);
}
}
}
