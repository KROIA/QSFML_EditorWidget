#pragma once
 
#include "OpticalElement.h"

class GlasRectangle : public OpticalElement
{
public:
    GlasRectangle(const std::string& name = "GlasRectangle");
    GlasRectangle(const GlasRectangle& other);
    ~GlasRectangle();
    OBJECT_DECL(GlasRectangle);


    void update() override;
private:
    
    class RectShape : public Shape
    {
    public:
        RectShape();

        bool getCollisionData(const QSFML::Utilities::Ray& ray,
            float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const override;
   
        void set(const sf::Vector2f& pos, const sf::Vector2f& size, float rotation);
        void setRotation(float angle);
        float getRotation() const;
        class Painter : public QSFML::Components::Drawable
        {
        public:
            Painter(RectShape* rect, const std::string& name = "Painter");
            Painter(const Painter& other);
            COMPONENT_DECL(Painter);

            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override;

            sf::Color m_color;
            RectShape* m_rect;
            unsigned int m_resolution;

        private:

        };
        Painter* m_painter;
        
    private:
        sf::Vector2f m_size;
        float m_angle;
        sf::Vector2f m_pos;

        sf::Vector2f m_TL, m_TR, m_BR, m_BL;

        
    };

    RectShape m_shape;
};