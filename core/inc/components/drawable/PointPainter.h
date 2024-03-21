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
                //std::vector<sf::Vertex> vertecies;
               // sf::VertexBuffer buffer;
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


            //void draw(sf::RenderTarget& target,
            //    sf::RenderStates states) const override;
            void draw(sf::RenderTarget& target,
                sf::RenderStates states) const override
            {
                //sf::VertexBuffer buffer(sf::TriangleFan);


               std::vector<sf::Vertex> shape(m_verteciesCount);
               float dAngle = 2 * M_PI / m_verteciesCount;
                
                for (auto& point : m_points)
                {

                    float angle = 0;
                    for (sf::Vertex& vertex : shape)
                    {
                        vertex.color = point.color;
                        sf::Vector2f& pos = vertex.position;
                        pos.x = cos(angle) * point.radius + point.position.x;
                        pos.y = sin(angle) * point.radius + point.position.y;
                        angle += dAngle;
                    }
                    //target.draw(point.buffer);
                    //target.draw(&point.vertecies[0], point.vertecies.size(), sf::TriangleFan, states);
                    target.draw(&shape[0], shape.size(), sf::TriangleFan, states);
                }


               
                /*sf::CircleShape circle(1, m_verteciesCount);
                circle.setOutlineThickness(0);
                for (auto& point : m_points)
                {
                    circle.setRadius(point.radius);
                    circle.setOrigin(point.radius, point.radius);
                    circle.setPosition(point.position);
                    circle.setFillColor(point.color);

                    target.draw(circle, states);
                }*/
            }

        private:
            /*void updateShape(PointData& data, float radius, const sf::Vector2f& pos)
            {
				float dAngle = 2 * M_PI / m_verteciesCount;
				float angle = 0;
				for (sf::Vertex& vertex : data.vertecies)
				{
					sf::Vector2f& pos = vertex.position;
					pos.x = cos(angle) * radius + pos.x;
					pos.y = sin(angle) * radius + pos.y;
					angle += dAngle;
				}
                //data.buffer.update(data.vertecies.data());
            }
            void updateColor(PointData& data, const sf::Color& color)
			{
				for (sf::Vertex& vertex : data.vertecies)
				{
					vertex.color = color;
				}
				//data.buffer.update(data.vertecies.data());
			}
            void updateAll(PointData& data, float radius, const sf::Vector2f& pos, const sf::Color& color)
			{
                float dAngle = 2 * M_PI / m_verteciesCount;
                float angle = 0;
                for (sf::Vertex& vertex : data.vertecies)
                {
                    vertex.color = color;
                    sf::Vector2f& pos = vertex.position;
                    pos.x = cos(angle) * radius + pos.x;
                    pos.y = sin(angle) * radius + pos.y;
                    angle += dAngle;
                }
                //data.buffer.update(data.vertecies.data());
			}*/

            float m_defaultRadius;
            size_t m_verteciesCount;
            sf::Color m_defaultColor;

            
            std::vector<PointData> m_points;
        };
    }
}
