#pragma once

#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"


class Shape
{
public:
    Shape() {}
    
    /*
        
        /return false if no collision with the shape occured
    */
    virtual bool getCollisionData(const QSFML::Utilities::Ray& ray, 
        float& outCollisionFactor, float& outNormalAngle, bool &rayStartsInsideShape) const = 0;
protected:

private:

};

class OpticalElement : public QSFML::Objects::CanvasObject
{
public:
    struct ReflectionAndRefractionData
    {
        float normalAngle;
        float reflectAngle;
        float refractAngle;
        bool doesRefract;
        float rayCollisionFactor;
    };

    OpticalElement(const std::string& name = "OpticalElement");
    OpticalElement(const OpticalElement& other);
    ~OpticalElement();
    OBJECT_DECL(OpticalElement);

    static const std::vector<OpticalElement*> const& getOpticalElements();

    void setShape(Shape* shape);
    Shape* getShape() const;

    void setRefractionIndexOutside(float n1);
    void setRefractionIndexInside(float n2);
    float getRefractionIndexInside() const;
    float getRefractionIndexOutside() const;

    bool processLaser(const QSFML::Utilities::Ray& ray, 
                              std::vector< QSFML::Utilities::Ray> & reflectedOut,
                              std::vector< LaserInfo> &additionalLightPathsOut) const;
    bool getRaycastDistance(const QSFML::Utilities::Ray& ray, float & outDistance) const;


    /* Canculates the reflection and refraction angle of the incomming ray on a face normal using
       the refractive index of the two media

       /param rayAngle: angle of the incomming ray in respect to the world.
       /param normalAngle: angle of the normal of the boundry layer in respect to the world.
                           The normal is always facing in to the optical denser object: n2>n1 -> normal facing in to n2
       /param n1: refraction index of the medium 1
       /param n2: refraction index of the medium 2
       /param outData: Reflection data
    */
    static void reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
        ReflectionAndRefractionData &outData);

    static bool reflectAndRefract(const QSFML::Utilities::Ray& ray, const Shape& shape, float n1, float n2,
        ReflectionAndRefractionData& outData);

    static bool reflectAndRefract_circleSegment(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
        float circleRadius, float minAngle, float maxAngle, float n1, float n2, bool normalDirToCircleCenter,
        float &outCollisionPointFactor,
        ReflectionAndRefractionData &outData);

    static bool reflectAndRefract_circle(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
        float circleRadius, float n1, float n2, bool normalDirToCircleCenter,
        float& outCollisionPointFactor,
        ReflectionAndRefractionData& outData);
    
    static bool reflectAndRefract_line(const QSFML::Utilities::Ray& ray, const sf::Vector2f& pos1, const sf::Vector2f& pos2,
        float& outCollisionPointFactor,
        ReflectionAndRefractionData& outData);
protected:
    //void update() override;

private:

    void processLaser_intern(const QSFML::Utilities::Ray& ray,
        std::vector< QSFML::Utilities::Ray>& reflectedOut,
        std::vector< LaserInfo>& additionalLightPathsOut,
        sf::Vector2f& outNextCollisionPoint) const;

    Shape* m_shape;
    float m_n1, m_n2;
    mutable size_t m_bounceCount;
    size_t m_maxBounceCount;
    static std::vector<OpticalElement*> s_opticalElements;

    
};