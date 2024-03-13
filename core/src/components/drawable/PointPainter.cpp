#include "components/drawable/PointPainter.h"
#include <SFML/Graphics.hpp>

namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(PointPainter);
        PointPainter::PointPainter(const std::string& name)
            : Drawable(name)
            , m_radius(1)
            , m_verteciesCount(16)
            , m_color(sf::Color::Green)
        {

        }
        PointPainter::PointPainter(const PointPainter& other)
            : Drawable(other)
            , m_radius(other.m_radius)
            , m_verteciesCount(other.m_verteciesCount)
            , m_color(other.m_color)
        {

        }

        void PointPainter::setRadius(float radius) 
        {
            m_radius = radius;
        }
        float PointPainter::getRadius() const
        {
            return m_radius;
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
            m_color = color;
        }
        const sf::Color& PointPainter::getColor() const
        {
            return m_color;
        }

        void PointPainter::setPoint(const sf::Vector2f& pos)
        {
            m_positions = { pos };
        }
        void PointPainter::setPoints(const std::vector<sf::Vector2f>& points)
        {
            m_positions = points;
        }
        void PointPainter::addPoint(const sf::Vector2f& pos)
        {
            m_positions.push_back(pos);
        }
        void PointPainter::clear()
        {
            m_positions.clear();
        }

        void PointPainter::draw(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            sf::CircleShape circle(m_radius, m_verteciesCount);
            circle.setFillColor(m_color);
            circle.setOutlineColor(m_color);
            circle.setOutlineThickness(0);
            sf::Vector2f radiusOffset(-m_radius, -m_radius);
            for (auto& pos : m_positions)
            {
                circle.setPosition(pos + radiusOffset);
                
                target.draw(circle, states);
            }
        }

    }
}
