#include "utilities/AABB.h"
#include <limits>

namespace QSFML
{
namespace Utilities
{
AABB::AABB()
{
    setPos({0,0});
    setSize({0,0});
}
AABB::AABB(const AABB &other)
{
    this->m_pos     =   other.m_pos;
    this->m_size    =   other.m_size;
}
AABB::AABB(const sf::Vector2f &pos, const sf::Vector2f &size)
{
    setPos(pos);
    setSize(size);
}
AABB::AABB(float x, float y, float width, float height)
{
    setPos({x,y});
    setSize({width,height});
}
AABB::~AABB()
{

}
const AABB &AABB::operator=(const AABB &other)
{
    this->m_pos     =   other.m_pos;
    this->m_size    =   other.m_size;
    return *this;
}
bool AABB::operator==(const AABB &other) const
{
    return this->m_size == other.m_size &&
           this->m_pos  == other.m_pos;
}
bool AABB::operator!=(const AABB &other) const
{
    return this->m_size != other.m_size ||
           this->m_pos  != other.m_pos;
}
bool AABB::operator<(const AABB &other) const
{
    return m_size.x * m_size.y < other.m_size.x * other.m_size.y;
}
bool AABB::operator>(const AABB &other) const
{
    return m_size.x * m_size.y > other.m_size.x * other.m_size.y;
}
bool AABB::operator<=(const AABB &other) const
{
    return m_size.x * m_size.y <= other.m_size.x * other.m_size.y;
}
bool AABB::operator>=(const AABB &other) const
{
    return m_size.x * m_size.y >= other.m_size.x * other.m_size.y;
}

void AABB::setPos(const sf::Vector2f &pos)
{
    m_pos = pos;
}
void AABB::setCenterPos(const sf::Vector2f &pos)
{
    setPos(pos - m_size/2.f);
}
void AABB::setX(float x)
{
    m_pos.x = x;
}
void AABB::setY(float y)
{
    m_pos.y = y;
}
void AABB::setSize(const sf::Vector2f &size)
{
    m_size = size;
#ifdef AABB_VALID_RECT_CHECK
    if(m_size.x < 0)
    {
        m_pos.x += m_size.x;
        m_size.x = -m_size.x;
    }
    if(m_size.y < 0)
    {
        m_pos.y += m_size.y;
        m_size.y = -m_size.y;
    }
#endif
}
void AABB::setWidth(float width)
{
    m_size.x = width;
#ifdef AABB_VALID_RECT_CHECK
    if(m_size.x < 0)
    {
        m_pos.x += m_size.x;
        m_size.x = -m_size.x;
    }
#endif
}
void AABB::setHeight(float height)
{
    m_size.y = height;
#ifdef AABB_VALID_RECT_CHECK
    if(m_size.y < 0)
    {
        m_pos.y += m_size.y;
        m_size.y = -m_size.y;
    }
#endif
}
void AABB::move(const sf::Vector2f &delta)
{
    m_pos += delta;
}
const sf::Vector2f &AABB::getPos() const
{
    return m_pos;
}
const sf::Vector2f &AABB::TL() const
{
    return m_pos;
}
sf::Vector2f AABB::TR() const
{
    sf::Vector2f p = m_pos;
    p.x += m_size.x;
    return p;
}
sf::Vector2f AABB::BR() const
{
    return m_pos + m_size;
}
sf::Vector2f AABB::BL() const
{
    sf::Vector2f p = m_pos;
    p.y += m_size.y;
    return p;
}
const sf::Vector2f &AABB::getSize() const
{
    return m_size;
}
sf::Vector2f AABB::getLeft() const
{
    return sf::Vector2f(0, m_size.y);
}
sf::Vector2f AABB::getTop() const
{
    return sf::Vector2f(m_size.x, 0);
}

bool AABB::contains(const sf::Vector2f &point) const
{
    return contains(*this,point);
}
bool AABB::intersects(const AABB &b) const
{
    return intersects(*this,b);
}
bool AABB::intersectsInverseOf(const AABB &b) const
{
    return intersectsInverseOf(*this,b);
}
bool AABB::isLeftOf(const AABB &b) const
{
    return isLeftOf(*this,b);
}
bool AABB::isRightOf(const AABB &b) const
{
     return isRightOf(*this,b);
}
bool AABB::isOnTopOf(const AABB &b) const
{
    return isOnTopOf(*this,b);
}
bool AABB::isBelowOf(const AABB &b) const
{
    return isBelowOf(*this,b);
}
bool AABB::intersectsTopOf(const AABB &b) const
{
    return intersectsTopOf(*this,b);
}
bool AABB::intersectsLeftOf(const AABB &b) const
{
    return intersectsLeftOf(*this,b);
}
bool AABB::intersectsBottomOf(const AABB &b) const
{
    return intersectsBottomOf(*this,b);
}
bool AABB::intersectsRightOf(const AABB &b) const
{
    return intersectsRightOf(*this,b);
}

bool AABB::contains(const AABB &a, const sf::Vector2f &point)
{
    return (a.m_pos.x <= point.x && a.m_pos.x + a.m_size.x >= point.x) &&
           (a.m_pos.y <= point.y && a.m_pos.y + a.m_size.y >= point.y);
}
bool AABB::intersects(const AABB &a, const AABB &b)
{
    return (a.m_pos.x <= b.m_pos.x + b.m_size.x && a.m_pos.x + a.m_size.x >= b.m_pos.x) &&
           (a.m_pos.y <= b.m_pos.y + b.m_size.y && a.m_pos.y + a.m_size.y >= b.m_pos.y);
}
bool AABB::intersectsInverseOf(const AABB &a, const AABB &b)
{
    return !(a.m_pos.x > b.m_pos.x && a.m_pos.x + a.m_size.x < b.m_pos.x + b.m_size.x &&
             a.m_pos.y > b.m_pos.y && a.m_pos.y + a.m_size.y < b.m_pos.y + b.m_size.y);
}
bool AABB::isLeftOf(const AABB &a, const AABB &b)
{
    return b.m_pos.x > a.m_pos.x + a.m_size.x;
}
bool AABB::isRightOf(const AABB &a, const AABB &b)
{
     return a.m_pos.x >  b.m_pos.x + b.m_size.x;
}
bool AABB::isOnTopOf(const AABB &a, const AABB &b)
{
    return b.m_pos.y > a.m_pos.y + a.m_size.y;
}
bool AABB::isBelowOf(const AABB &a, const AABB &b)
{
    return a.m_pos.y >  b.m_pos.y + b.m_size.y;
}
bool AABB::intersectsTopOf(const AABB &a, const AABB &b)
{
    return a.m_pos.y < b.m_pos.y && a.m_pos.y+a.m_size.y > b.m_pos.y;
}
bool AABB::intersectsLeftOf(const AABB &a, const AABB &b)
{
    return a.m_pos.x < b.m_pos.x && a.m_pos.x+a.m_size.x > b.m_pos.x;
}
bool AABB::intersectsBottomOf(const AABB &a, const AABB &b)
{
    return a.m_pos.y < b.m_pos.y + b.m_size.y && a.m_pos.y + a.m_size.y > b.m_pos.y + b.m_size.y;
}
bool AABB::intersectsRightOf(const AABB &a, const AABB &b)
{
    return a.m_pos.x < b.m_pos.x + b.m_size.x && a.m_pos.x + a.m_size.x > b.m_pos.x + b.m_size.x;
}

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

        if(max.x < size.x)
            max.x = size.x;
        if(max.y < size.y)
            max.y = size.y;
    }
    return AABB(min,max);
}
}
}
