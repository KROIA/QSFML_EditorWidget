#pragma once

#include "QSFML_EditorWidget.h"
#include "LaserInfo.h"
#include "LightRay.h"

class Shape
{
public:
    Shape() {}
    
    /*
        
        /return false if no collision with the shape occured
    */
    virtual bool getCollisionData(const LightRay& ray,
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

    void doesReflect(bool enable);
    void doesRefract(bool enable);
    bool doesReflect() const;
    bool doesRefract() const;

    bool processLaser(const LightRay& ray, 
                              std::vector< LightRay> & reflectedOut,
                              std::vector< LaserInfo> &additionalLightPathsOut) const;
    bool getRaycastDistance(const LightRay& ray, float & outDistance) const;


    /* Canculates the reflection and refraction angle of the incomming ray on a face normal using
       the refractive index of the two media

       /param rayAngle: angle of the incomming ray in respect to the world.
       /param normalAngle: angle of the normal of the boundry layer in respect to the world.
                           The normal is always facing in to the optical denser object: n2>n1 -> normal facing in to n2
       /param n1: refraction index of the medium 1
       /param n2: refraction index of the medium 2
       /param outData: Reflection data
    */
    void reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
        ReflectionAndRefractionData &outData) const;

    bool reflectAndRefract(const LightRay& ray, const Shape& shape, float n1, float n2,
        ReflectionAndRefractionData& outData) const;

protected:
    //void update() override;

private:

    void processLaser_intern(const LightRay& ray,
        std::vector< LightRay>& reflectedOut,
        std::vector< LaserInfo>& additionalLightPathsOut,
        sf::Vector2f& outNextCollisionPoint) const;

    Shape* m_shape;
    float m_n1, m_n2;
    bool m_doesReflect; 
    bool m_doesRefract;
    mutable size_t m_bounceCount;
    size_t m_maxBounceCount;
    static std::vector<OpticalElement*> s_opticalElements;

    
};