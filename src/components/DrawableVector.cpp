#include <SFML/Graphics.hpp>
#include "components/DrawableVector.h"
#include "math.h"

using namespace QSFML::Components;

DrawableVector::DrawableVector(const std::string &name)
    :   Drawable(name)
{

}
DrawableVector::DrawableVector(const DrawableVector &other)
    :   Drawable(other)
{
    m_start = other.m_start;
    m_end = other.m_end;
    m_color = other.m_color;
}

void DrawableVector::setPoints(const sf::Vector2f &begin, const sf::Vector2f &end)
{
    m_start = begin;
    m_end = end;
}
void DrawableVector::setStart(const sf::Vector2f &begin)
{
    m_start = begin;
}
void DrawableVector::setEnd(const sf::Vector2f &end)
{
    m_end = end;
}
void DrawableVector::setDirection(const sf::Vector2f &directionVec)
{
    m_end = m_start + directionVec;
}
void DrawableVector::setColor(const sf::Color &color)
{
    m_color = color;
}

const sf::Vector2f &DrawableVector::getStart() const
{
    return m_start;
}
const sf::Vector2f &DrawableVector::getEnd() const
{
    return m_end;
}
const sf::Color &DrawableVector::getColor() const
{
    return m_color;
}
sf::Vector2f DrawableVector::getDirection() const
{
    return (m_end - m_start);
}


void DrawableVector::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const
{
    sf::Vector2f dir = getDirection();
    float length = sqrt(dir.x*dir.x + dir.y*dir.y);
    float angle = getAngle(dir);

    static const float arrowTipAngle = M_PI *4.f / 5.f;

    if(length < 30)
        length = 30;
    else if(length > 200)
        length = 200;
    sf::Vector2f arrowTip(0.1f*length,0);
    sf::Vector2f arrowLeft = getRotated(arrowTip,angle - arrowTipAngle);
    sf::Vector2f arrowRight = getRotated(arrowTip,angle+ arrowTipAngle);

    sf::Vertex lines[] =
    {
        sf::Vertex(m_start,m_color),
        sf::Vertex(m_end,m_color),

        sf::Vertex(m_end,m_color),
        sf::Vertex(m_end+arrowLeft,m_color),

        sf::Vertex(m_end,m_color),
        sf::Vertex(m_end+arrowRight,m_color),
    };
    target.draw(lines, 6, sf::Lines);
}
sf::Vector2f DrawableVector::getRotated(const sf::Vector2f &vec,
                                        float angle) const
{
    float c = cos(angle);
    float s = sin(angle);
    return sf::Vector2f(c * vec.x - s * vec.y,
                       s * vec.x + c * vec.y);
}
float DrawableVector::getLength(const sf::Vector2f &vec) const
{
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}
float DrawableVector::getAngle(const sf::Vector2f &vec) const
{
    float l = getLength(vec);
    if(l == 0)
        return 0;
    if(vec.x < 0)
        return M_PI - asin(vec.y/l);
    return asin(vec.y/l);
}
