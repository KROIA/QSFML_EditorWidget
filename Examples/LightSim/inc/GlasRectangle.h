#pragma once
 
#include "OpticalElement.h"

class GlasRectangle : public OpticalElement
{
public:
    GlasRectangle(const std::string& name = "GlasRectangle");
    GlasRectangle(const GlasRectangle& other);
    ~GlasRectangle();
    OBJECT_DECL(GlasRectangle);


    bool processLaser(const QSFML::Utilities::Ray& ray,
        std::vector< QSFML::Utilities::Ray>& reflectedOut,
        std::vector< LaserInfo>& additionalLightPathsOut) const override;

    bool getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const override;


private:
    bool processLaser_intern(const QSFML::Utilities::Ray& ray,
        std::vector< QSFML::Utilities::Ray>& reflectedOut,
        std::vector< LaserInfo>& additionalLightPathsOut,
        bool outgoingLaser, sf::Vector2f &outCollisionPoint) const;

    QSFML::Utilities::AABB m_box;
    float m_n1;
    float m_n2;
};