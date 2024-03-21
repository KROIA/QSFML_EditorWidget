#include "components/drawable/PointPainter.h"
#include <SFML/Graphics.hpp>

namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(PointPainter);
        PointPainter::PointPainter(const std::string& name)
            : Drawable(name)
            , m_defaultRadius(1)
            , m_verteciesCount(8)
            , m_defaultColor(sf::Color::White)
        {

        }
        PointPainter::PointPainter(const PointPainter& other)
            : Drawable(other)
            , m_defaultRadius(other.m_defaultRadius)
            , m_verteciesCount(other.m_verteciesCount)
            , m_defaultColor(other.m_defaultColor)
            , m_points(other.m_points)
        {

        }

        void PointPainter::setRadius(float radius) 
        {
            m_defaultRadius = radius;
            for (auto& point : m_points)
            {
                //updateShape(point, radius, point.position);
                point.radius = radius;
            }
        }
        void PointPainter::setRadius(size_t index, float radius)
        {
            m_points[index].radius = radius;
            //updateShape(m_points[index], radius, m_points[index].position);
        }
        float PointPainter::getRadius() const
        {
            return m_defaultRadius;
        }
        float PointPainter::getRadius(size_t index) const
        {
            return m_points[index].radius;
            //return m_points[index].vertecies[0].position.x - m_points[index].position.x;
        }

        void PointPainter::setVerteciesCount(size_t count)
        {
            m_verteciesCount = count;
        }
        size_t PointPainter::getVerteciesCount() const
        {
            return m_verteciesCount;
        }

        void PointPainter::setColor(const sf::Color& color)
        {
            m_defaultColor = color;
            for (auto& point : m_points)
            {
                //updateColor(point, color);
				point.color = color;
			}
        }
        void PointPainter::setColor(size_t index, const sf::Color& color)
        {
            //updateColor(m_points[index], color);
		    m_points[index].color = color;
        }
        const sf::Color& PointPainter::getColor() const
        {
            return m_defaultColor;
        }
        const sf::Color& PointPainter::getColor(size_t index) const
        {
            //return m_points[index].vertecies[0].color;
            return m_points[index].color;
        }

        void PointPainter::setPoint(const sf::Vector2f& pos)
        {
            //m_points = { {pos, m_defaultColor, m_defaultRadius} };
            m_points = { {pos, m_defaultColor, m_defaultRadius}};
            //m_points.back().buffer.create(m_verteciesCount);
           // updateShape(m_points[0], m_defaultRadius, pos);
        }
        void PointPainter::setPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            m_points = { {pos, color, radius} };
           // m_points.back().buffer.create(m_verteciesCount);
            //updateAll(m_points[0], radius, pos, color);
        }
        void PointPainter::setPoints(const std::vector<sf::Vector2f>& points)
        {
            m_points.clear();
            m_points.reserve(points.size());
            for (auto& pos : points)
            {
				//m_points.push_back({ pos, m_defaultColor, m_defaultRadius });
				m_points.push_back({ pos, m_defaultColor, m_defaultRadius });
                //updateAll(m_points.back(), m_defaultRadius, pos, m_defaultColor);
                //m_points.back().buffer.create(m_verteciesCount);
			}
        }
        void PointPainter::addPoint(const sf::Vector2f& pos)
        {
            //m_points.push_back({pos, m_defaultColor, m_defaultRadius});
            m_points.push_back({ pos, m_defaultColor, m_defaultRadius});
            //m_points.back().buffer.create(m_verteciesCount);
        }
        void PointPainter::addPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            //m_points.push_back({ pos, color, radius });
            m_points.push_back({ pos, m_defaultColor, m_defaultRadius});
            //m_points.back().buffer.create(m_verteciesCount);
           // updateAll(m_points.back(), radius, pos, color);
        }
        PointPainter::PointData& PointPainter::getPoint(size_t index)
        {
            return m_points[index];
        }
        void PointPainter::setPoint(size_t index, const PointData& data)
        {
            m_points[index] = data;
        }
        void PointPainter::clear()
        {
            m_points.clear();
        }

        /*
        void PointPainter::draw(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            sf::CircleShape circle(1,m_verteciesCount);
            circle.setOutlineThickness(0);
            for (auto& point : m_points)
            {
                circle.setRadius(point.radius);
                circle.setOrigin(point.radius, point.radius);
                circle.setPosition(point.position);
                circle.setFillColor(point.color);
                
                target.draw(circle, states);
            }
        }
        */
    }
}
