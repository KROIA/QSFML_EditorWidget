#pragma once

#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"


class OpticalElement : public QSFML::Objects::CanvasObject
{
public:
    OpticalElement(const std::string& name = "OpticalElement");
    OpticalElement(const OpticalElement& other);
    ~OpticalElement();
    OBJECT_DECL(OpticalElement);

    static const std::vector<OpticalElement*> const& getOpticalElements();

    virtual bool processLaser(const QSFML::Utilities::Ray& ray, 
                              std::vector< QSFML::Utilities::Ray> & reflectedOut,
                              std::vector< LaserInfo> &additionalLightPathsOut) const;
    virtual bool getRaycastDistance(const QSFML::Utilities::Ray& ray, float &distanceOut) const;


    /* Canculates the reflection and refraction angle of the incomming ray on a face normal using
       the refractive index of the two media

       /param rayAngle: angle of the incomming ray in respect to the world.
       /param normalAngle: angle of the normal of the boundry layer in respect to the world.
                           The normal is always facing in to the optical denser object: n2>n1 -> normal facing in to n2
       /param n1: refraction index of the medium 1
       /param n2: refraction index of the medium 2
       /param outReflectAngle: the global angle of the reflected part of the light.
       /param outRefractAngle: the global angle of the refracted part of the light, if it exists.
       /param outHasRefraction: true, if it has a refraction, otherwise false.
    */
    static void reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
        float& outReflectAngle, float& outRefractAngle, bool &outHasRefraction);

    static bool reflectAndRefract_circleSegment(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
        float circleRadius, float minAngle, float maxAngle, float n1, float n2, bool normalDirToCircleCenter,
        float &outCollisionPointFactor,
        float& outCollisionNormalAngle, float& outReflectAngle, float& outRefractAngle, bool& outHasRefraction);

    static bool reflectAndRefract_circle(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
        float circleRadius, float n1, float n2, bool normalDirToCircleCenter,
        float& outCollisionPointFactor,
        float& outCollisionNormalAngle, float& outReflectAngle, float& outRefractAngle, bool& outHasRefraction);

protected:
    //void update() override;

private:

    static std::vector<OpticalElement*> s_opticalElements;
    
};