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

    max.x = -std::numeric_limits<float>::max();
    max.y = -std::numeric_limits<float>::max();
    for(size_t i=0; i<list.size(); i++)
    {
        const sf::Vector2f &pos1 = list[i].m_pos;
        sf::Vector2f pos2 = list[i].m_size + pos1;
        if(min.x > pos1.x)
            min.x = pos1.x;
        if(min.y > pos1.y)
            min.y = pos1.y;

        if(max.x < pos2.x)
            max.x = pos2.x;
        if(max.y < pos2.y)
            max.y = pos2.y;
    }
    return AABB(min, max - min);
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
