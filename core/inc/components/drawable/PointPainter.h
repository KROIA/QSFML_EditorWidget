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
            PointPainter(const std::string& name = "PointPainter");
            PointPainter(const PointPainter& other);
            COMPONENT_DECL(PointPainter);

            void setRadius(float radius);
            float getRadius() const;
            
            void setVerteciesCount(size_t count);
            size_t getVerteciesCount() const;

            void setColor(const sf::Color& color);
            const sf::Color& getColor() const;

            void setPoint(const sf::Vector2f& pos);
            void setPoints(const std::vector<sf::Vector2f>& points);
            void addPoint(const sf::Vector2f& pos);
            void clear();

            void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

        private:
            float m_radius;
            size_t m_verteciesCount;
            sf::Color m_color;

            std::vector<sf::Vector2f> m_positions;
        };
    }
}
