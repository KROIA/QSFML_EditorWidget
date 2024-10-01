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
            class PointData
            {
				friend PointPainter;
                public:
				PointData(const sf::Vector2f& pos = sf::Vector2f(0, 0),
						  float radius = 1.0f,
						  const sf::Color& color = sf::Color::White)
					: m_position(pos), m_radius(radius), m_color(color) { }

                void setPosition(const sf::Vector2f& pos) { m_position = pos; m_isDirty = true; }
				const sf::Vector2f& getPosition() const { return m_position; }
				void setColor(const sf::Color& color) { m_color = color; m_isDirty = true; }
				const sf::Color& getColor() const { return m_color; }
				void setRadius(float radius) { m_radius = radius; m_isDirty = true; }
				float getRadius() const { return m_radius; }

                private:
                sf::Vector2f m_position;
                sf::Color m_color;
                float m_radius;

                void updateVertecies(size_t vertexCount) const;
				mutable bool m_isDirty = true;
				mutable sf::VertexArray m_vertecies;
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

            /// <summary>
            /// Setting to switch from relative to global position
            /// </summary>
            /// <description>
            /// Using relative position means that the position of the points is relative to the parent object.
            /// Using global position means that the position of the points is relative to the world.
            /// </description>
            /// <param name="useGlobal"></param>
            void useGlobalPosition(bool useGlobal)
            {
                m_useGlobalPosition = useGlobal;
            }

            /// <summary>
            /// Gets the positioning mode
            /// </summary>
            /// <returns>true if the mode is global positioning</returns>
            bool isUsingGlobalPosition() const
            {
                return m_useGlobalPosition;
            }

           

        private: 
        void drawComponent(sf::RenderTarget& target,
                           sf::RenderStates states) const override;

            float m_defaultRadius;
            size_t m_verteciesCount;
            sf::Color m_defaultColor;
            bool m_useGlobalPosition = false;

            
            std::vector<PointData> m_points;
        };
    }
}
