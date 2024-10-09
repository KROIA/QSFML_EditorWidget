#include "components/drawable/PointPainter.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace QSFML
{
    namespace Components
    {
        void PointPainter::PointData::updateVertecies(size_t vertexCount) const
        {
			if (m_isDirty)
			{
                //QSFMLP_COMPONENT_BLOCK("PointPainter::PointData::updateVertecies", QSFML_COLOR_STAGE_2);
				//QSFMLP_COMPONENT_VALUE("vertexCount", vertexCount);
#ifdef QSFML_USE_GL_DRAW
				if (m_vertecies.size() != vertexCount)
#else
                if (m_vertecies.getVertexCount() != vertexCount)
#endif
                {
                    m_vertecies.clear();
					m_vertecies.resize(vertexCount);
                }
#ifndef QSFML_USE_GL_DRAW
				m_vertecies.setPrimitiveType(sf::PrimitiveType::TriangleFan);
#endif
				
				float dAngle = (float)(2 * M_PI / vertexCount);
				float angle = 0;
#ifdef QSFML_USE_GL_DRAW
				for (size_t i = 0; i < vertexCount; ++i)
#else
				for (size_t i=0; i< m_vertecies.getVertexCount(); ++i)
#endif
				{
#ifdef QSFML_USE_GL_DRAW
					sf::Vector2f& pos = m_vertecies[i];
#else
					sf::Vertex& vertex = m_vertecies[i];
					vertex.color = m_color;
					sf::Vector2f& pos = vertex.position;
#endif
					pos.x = cos(angle) * m_radius + m_position.x;
					pos.y = sin(angle) * m_radius + m_position.y;
					angle += dAngle;
				}
				m_isDirty = false;
			}
        }


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
                point.setRadius(radius);
            }
        }
        void PointPainter::setRadius(size_t index, float radius)
        {
            m_points[index].setRadius(radius);
        }
        float PointPainter::getRadius() const
        {
            return m_defaultRadius;
        }
        float PointPainter::getRadius(size_t index) const
        {
            return m_points[index].getRadius();
        }

        void PointPainter::setVerteciesCount(size_t count)
        {
			if (m_verteciesCount == count) return;
            m_verteciesCount = count;
			for (auto& point : m_points)
			{
                point.m_isDirty = true;
			}
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
				point.setColor(color);
			}
        }
        void PointPainter::setColor(size_t index, const sf::Color& color)
        {
		    m_points[index].setColor(color);
        }
        const sf::Color& PointPainter::getColor() const
        {
            return m_defaultColor;
        }
        const sf::Color& PointPainter::getColor(size_t index) const
        {
            return m_points[index].getColor();
        }

        void PointPainter::setPoint(const sf::Vector2f& pos)
        {
            m_points = { PointData(pos, m_defaultRadius, m_defaultColor) };
        }
        void PointPainter::setPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            m_points = { PointData(pos, radius, color) };
        }
        void PointPainter::setPoints(const std::vector<sf::Vector2f>& points)
        {
            m_points.clear();
            m_points.reserve(points.size());
            for (auto& pos : points)
            {
				m_points.push_back(PointData( pos, m_defaultRadius, m_defaultColor));
			}
        }
        void PointPainter::addPoint(const sf::Vector2f& pos)
        {
            m_points.push_back(PointData(pos, m_defaultRadius, m_defaultColor));
        }
        void PointPainter::addPoint(const sf::Vector2f& pos, float radius, const sf::Color& color)
        {
            m_points.push_back(PointData(pos, radius, color));
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


        void PointPainter::drawComponent(sf::RenderTarget& target,
                           sf::RenderStates states) const
        {
            QSFMLP_COMPONENT_FUNCTION(QSFML_COLOR_STAGE_1);
            QSFMLP_COMPONENT_VALUE("pointCount", m_points.size());
            if (m_useGlobalPosition)
                states = sf::RenderStates();
#ifdef QSFML_USE_GL_DRAW
            QSFML_UNUSED(target);
            glLoadMatrixf(states.transform.getMatrix());
            sf::Color previousColor = sf::Color(0, 0, 0, 0);
            
            for (auto& point : m_points)
            {
                if (point.m_isDirty)
                    point.updateVertecies(m_verteciesCount);
                glBegin(GL_TRIANGLE_FAN);
				//glColor4ub(point.m_color.r, point.m_color.g, point.m_color.b, point.m_color.a);
                if (point.m_color != previousColor)
                {
                    glColor4ub(point.m_color.r, point.m_color.g, point.m_color.b, point.m_color.a);
                    previousColor = point.m_color;
                }
				for (auto& vertex : point.m_vertecies)
				{
					glVertex2f(vertex.x, vertex.y);
				}
                glEnd();
            }
            
#else
            for (auto& point : m_points)
            {
				if (point.m_isDirty)
				    point.updateVertecies(m_verteciesCount);
                target.draw(point.m_vertecies, states);                
            }
#endif
        }
    }
}
