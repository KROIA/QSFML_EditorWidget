#include "GlasRectangle.h"

GlasRectangle::GlasRectangle(const std::string& name)
    : OpticalElement(name)
{
    m_shape.m_painter = new RectShape::Painter(&m_shape);
    m_shape.set(sf::Vector2f(100, 200), sf::Vector2f(50, 100), 0);

    setRefractionIndexOutside(1);
    setRefractionIndexInside(1.5);
    setShape(&m_shape);
    addComponent(m_shape.m_painter);
}
GlasRectangle::GlasRectangle(const GlasRectangle& other)
    : OpticalElement(other)
    , m_shape(other.m_shape)
{
    setShape(&m_shape);
}
GlasRectangle::~GlasRectangle()
{

}
OBJECT_IMPL(GlasRectangle);

void GlasRectangle::update()
{
    m_shape.setRotation(m_shape.getRotation() + M_PI * 0.001) ;
}

GlasRectangle::RectShape::RectShape()
    : Shape()
    , m_angle(0)
{

}


bool GlasRectangle::RectShape::getCollisionData(const LightRay& ray,
    float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{

    float d1 = 999999, d2 = 999999, d3 = 999999, d4 = 999999;
    using namespace QSFML::Utilities;
    float fac1 = 999999, fac2 = 999999, fac3 = 999999, fac4 = 999999;
    bool c1 = ray.ray.raycast(Ray(m_TL, m_TR - m_TL), d1, fac1);
    bool c2 = ray.ray.raycast(Ray(m_TL, m_BL - m_TL), d2, fac2);
    bool c3 = ray.ray.raycast(Ray(m_BR, m_TR - m_BR), d3, fac3);
    bool c4 = ray.ray.raycast(Ray(m_BR, m_BL - m_BR), d4, fac4);

    if (fac1 < 0 || fac1 > 1 || d1 < 0)
        c1 = false;
    if (fac2 < 0 || fac2 > 1 || d2 < 0)
        c2 = false;
    if (fac3 < 0 || fac3 > 1 || d3 < 0)
        c3 = false;
    if (fac4 < 0 || fac4 > 1 || d4 < 0)
        c4 = false;

    if (!(c1 + c2 + c3 + c4))
        return false;

    if ((d1 < 0 && d4 > 0 && d2 < 0 && d3 > 0) ||
        (d1 > 0 && d4 < 0 && d2 > 0 && d3 < 0) || 
        (d1 > 0 && d4 < 0 && d2 < 0 && d3 > 0) ||
        (d1 < 0 && d4 > 0 && d2 > 0 && d3 < 0))
        rayStartsInsideShape = true;
    else
        rayStartsInsideShape = false;

    float shortest = 9999999;
    float coFac = 9999999;

   

    if (c1)
    {
        shortest = d1;
        coFac = fac1;
        outNormalAngle = M_PI_2;
    }
    if (c2 && shortest > d2)
    {
        shortest = d2;
        coFac = fac2;
        outNormalAngle = 0;
    }
    if (c3 && shortest > d3)
    {
        shortest = d3;
        coFac = fac3;
        outNormalAngle = -M_PI;
    }
    if (c4 && shortest > d4)
    {
        shortest = d4;
        coFac = fac4;
        outNormalAngle = -M_PI_2;
    }
    outNormalAngle += m_angle;
    outCollisionFactor = shortest;
    return true;

}
void GlasRectangle::RectShape::setRotation(float angle)
{
    set(m_pos, m_size, angle);
}
float GlasRectangle::RectShape::getRotation() const
{
    return m_angle;
}
void GlasRectangle::RectShape::set(const sf::Vector2f& pos, const sf::Vector2f& size, float rotation)
{
    float width2 = size.x / 2;
    float height2 = size.y / 2;
    m_TL.x = -width2;
    m_TL.y = -height2;

    m_TR.x = width2;
    m_TR.y = -height2;

    m_BR.x = width2;
    m_BR.y = height2;

    m_BL.x = -width2;
    m_BL.y = height2;

    m_pos = pos;
    m_size = size;
    m_angle = rotation;

    m_TL = QSFML::VectorMath::getRotatedRAD(m_TL, m_angle) + m_pos;
    m_TR = QSFML::VectorMath::getRotatedRAD(m_TR, m_angle) + m_pos;
    m_BR = QSFML::VectorMath::getRotatedRAD(m_BR, m_angle) + m_pos;
    m_BL = QSFML::VectorMath::getRotatedRAD(m_BL, m_angle) + m_pos;
}

GlasRectangle::RectShape::Painter::Painter(RectShape* rect, const std::string& name)
    : Drawable(name)
    , m_rect(rect)
{

}
GlasRectangle::RectShape::Painter::Painter(const Painter& other)
    : Drawable(other)
    , m_rect(nullptr)
{

}
COMPONENT_IMPL(GlasRectangle::RectShape::Painter);

void GlasRectangle::RectShape::Painter::drawComponent(sf::RenderTarget& target,
    sf::RenderStates states) const
{
    sf::Vertex line[] =
    {
        sf::Vertex(m_rect->m_TL),
        sf::Vertex(m_rect->m_TR),
        sf::Vertex(m_rect->m_BR),
        sf::Vertex(m_rect->m_BL),
        sf::Vertex(m_rect->m_TL),
    };

    target.draw(line, 5, sf::LinesStrip);
}
