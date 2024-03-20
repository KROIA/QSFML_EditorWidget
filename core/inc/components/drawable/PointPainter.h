#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"

#include <vector>

namespace QSFML
{
    namespace Components
    {
        class QSFML_EDITOR_WIDGET_EXPORT PointPainter : public Drawable
        {
        public:
            struct PointData
            {
                sf::Vector2f position;
                sf::Color color;
                float radius;
            };

            PointPainter(const std::string& name = "PointPainter");
            PointPainter(const PointPainter& other);
            COMPONENT_DECL(PointPainter);

            void setRadius(float radius);
            void setRadius(size_t index, float radius);
            float getRadius() const;
            float getRadius(size_t index) const;
            
            void setVerteciesCount(size_t count);
            size_t getVerteciesCount() const;

            void setColor(const sf::Color& color);
            void setColor(size_t index, const sf::Color& color);
            const sf::Color& getColor() const;
            const sf::Color& getColor(size_t index) const;

            void setPoint(const sf::Vector2f& pos);
            void setPoint(const sf::Vector2f&pos, float radius, const sf::Color& color);
            void setPoints(const std::vector<sf::Vector2f>& points);
            void addPoint(const sf::Vector2f& pos);
            void addPoint(const sf::Vector2f& pos, float radius, const sf::Color& color);

            PointData& getPoint(size_t index);
            void setPoint(size_t index, const PointData& data);

            void clear();


            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override;

        private:
            float m_defaultRadius;
            size_t m_verteciesCount;
            sf::Color m_defaultColor;

            
            std::vector<PointData> m_points;
        };
    }
}
