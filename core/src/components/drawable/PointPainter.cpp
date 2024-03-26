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
                point.radius = radius;
            }
        }
        void PointPainter::setRadius(size_t index, float radius)
        {
            m_points[index].radius = radius;
        }
        float PointPainter::getRadius() const
        {
            return m_defaultRadius;
        }
        float PointPainter::getRadius(size_t index) const
        {
            return m_points[index].radius;
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
				point.color = color;
			}
        }
        void PointPainter::setColor(size_t index, const sf::Color& color)
        {
		    m_points[index].color = color;
        }
        const sf::Color& PointPainter::getColor() const
        {
            return m_defaultColor;
        }
        const sf::Color& PointPainter::getColor(size_t index) const
        {
            return m_points[index].color;
        }

        void PointPainter::setPoint(const sf::Vector2f& pos)
        {
            m_points = { {pos, m_defaultColor, m_defaultRadius}};
        }
        void PointPainter::setPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            m_points = { {pos, color, radius} };
        }
        void PointPainter::setPoints(const std::vector<sf::Vector2f>& points)
        {
            m_points.clear();
            m_points.reserve(points.size());
            for (auto& pos : points)
            {
				m_points.push_back({ pos, m_defaultColor, m_defaultRadius });
			}
        }
        void PointPainter::addPoint(const sf::Vector2f& pos)
        {
            m_points.push_back({ pos, m_defaultColor, m_defaultRadius});
        }
        void PointPainter::addPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            m_points.push_back({ pos, color, radius});
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
    }
}
