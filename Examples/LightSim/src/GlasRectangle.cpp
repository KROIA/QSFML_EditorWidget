#include "GlasRectangle.h"

GlasRectangle::GlasRectangle(const std::string& name)
    : OpticalElement(name)
{
    m_box.setPos(sf::Vector2f(100, 100));
    m_box.setSize(sf::Vector2f(50, 300));

    m_n1 = 1;
    m_n2 = 1.5;

}
GlasRectangle::GlasRectangle(const GlasRectangle& other)
    : OpticalElement(other)
{

}
GlasRectangle::~GlasRectangle()
{

}
OBJECT_IMPL(GlasRectangle);


bool GlasRectangle::processLaser(const QSFML::Utilities::Ray& ray,
    std::vector< QSFML::Utilities::Ray>& reflectedOut,
    std::vector< LaserInfo>& additionalLightPathsOut) const
{
    sf::Vector2f collisionPoint; 
    return processLaser_intern(ray, reflectedOut, additionalLightPathsOut, false, collisionPoint);
}

bool GlasRectangle::processLaser_intern(const QSFML::Utilities::Ray& ray,
    std::vector< QSFML::Utilities::Ray>& reflectedOut,
    std::vector< LaserInfo>& additionalLightPathsOut,
    bool outgoingLaser, sf::Vector2f& outCollisionPoint) const
{
    float d1 = 999999, d2 = 999999, d3 = 999999, d4 = 999999;
    using namespace QSFML::Utilities;
    float fac1 = 999999, fac2 = 999999, fac3 = 999999, fac4 = 999999;
    bool c1 = ray.raycast(Ray(m_box.getPos(), m_box.TR() - m_box.TL()), d1, fac1);
    bool c2 = ray.raycast(Ray(m_box.getPos(), m_box.BL() - m_box.TL()), d2, fac2);
    bool c3 = ray.raycast(Ray(m_box.BR(), m_box.TR() - m_box.BR()), d3, fac3);
    bool c4 = ray.raycast(Ray(m_box.BR(), m_box.BL() - m_box.BR()), d4, fac4);

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


    float shortest = 9999999;
    float coFac = 9999999;

    sf::Vector2f normal;

    if (c1)
    {
        shortest = d1;
        coFac = fac1;
        normal = sf::Vector2f(0, 1);
    }
    if (c2 && shortest > d2)
    {
        shortest = d2;
        coFac = fac2;
        normal = sf::Vector2f(1, 0);
    }
    if (c3 && shortest > d3)
    {
        shortest = d3;
        coFac = fac3;
        normal = sf::Vector2f(-1, 0);
    }
    if (c4 && shortest > d4)
    {
        shortest = d4;
        coFac = fac4;
        normal = sf::Vector2f(0, -1);
    }

    outCollisionPoint = ray.getPoint(shortest);

    float rayAngle = QSFML::VectorMath::getAngle(ray.getDirection());
    float normalAngle = QSFML::VectorMath::getAngle(normal);

    float reflectAngle1, refractAngle1;
    bool hasRefraction1;


    float n1 = m_n1;
    float n2 = m_n2;
    if (outgoingLaser)
    {
        n1 = m_n2;
        n2 = m_n1;
    }
    reflectAndRefract(rayAngle, normalAngle, n1, n2, reflectAngle1, refractAngle1, hasRefraction1);
    
    if (outgoingLaser && !hasRefraction1)
    {
        QSFML::Utilities::Ray reflected(ray);
        sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVector(reflectAngle1);
        reflected.setPos(outCollisionPoint + 0.001f * dir);
        reflected.setDirection(dir);

        reflectedOut.push_back(reflected);
        //LaserInfo info;
        //info.start = reflected.getPos();
        //info.end = reflected.getPoint(100);
        //additionalLightPathsOut.push_back(info);
    }
    if (hasRefraction1)
    {
        QSFML::Utilities::Ray refracted(ray);
        sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVector(refractAngle1);

        refracted.setDirection(dir);

        if (outgoingLaser)
        {
            refracted.setPos(outCollisionPoint + 0.001f * dir);
            reflectedOut.push_back(refracted);
            return true;
        }
        else
        {
            refracted.setPos(outCollisionPoint);
            LaserInfo info;
            info.start = refracted.getPos();


            refracted.setPos(outCollisionPoint + 0.001f * dir);
            sf::Vector2f collisionPoint1;
            processLaser_intern(refracted, reflectedOut, additionalLightPathsOut, true, collisionPoint1);
            if (collisionPoint1.x == 0)
            {
                int a = 0;
            }
            info.end = collisionPoint1;
            additionalLightPathsOut.push_back(info);
            return true;
        }
    }


    return false;
}

bool GlasRectangle::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
    float d1 = 999999, d2 = 999999, d3 = 999999, d4 = 999999;
    using namespace QSFML::Utilities;
    float fac1 = 999999, fac2 = 999999, fac3 = 999999, fac4 = 999999;
    bool c1 = ray.raycast(Ray(m_box.getPos(), m_box.TR() - m_box.TL()), d1, fac1);
    bool c2 = ray.raycast(Ray(m_box.getPos(), m_box.BL() - m_box.TL()), d2, fac2);
    bool c3 = ray.raycast(Ray(m_box.BR(), m_box.TR() - m_box.BR()), d3, fac3);
    bool c4 = ray.raycast(Ray(m_box.BR(), m_box.BL() - m_box.BR()), d4, fac4);

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


    float shortest = 9999999;
    float coFac = 9999999;

    if (c1)
    {
        shortest = d1;
        coFac = fac1;
    }
    if (c2 && shortest > d2)
    {
        shortest = d2;
        coFac = fac2;
    }
    if (c3 && shortest > d3)
    {
        shortest = d3;
        coFac = fac3;
    }
    if (c4 && shortest > d4)
    {
        shortest = d4;
        coFac = fac4;
    }

    distanceOut = shortest;
    return true;
}