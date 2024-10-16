#include "components/drawable/PathPainter.h"
#include <math.h>
#include <SFML/Graphics.hpp>

#include <SFML/OpenGL.hpp>

namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(PathPainter)
            PathPainter::PathPainter(const std::string& name)
            : Drawable(name)
            , m_thickness(2)
            , m_color(sf::Color::Green)
        {

        }
        PathPainter::PathPainter(const PathPainter& other)
            : Drawable(other)
            , m_thickness(other.m_thickness)
            , m_color(other.m_color)
            , m_vertecies(other.m_vertecies)
        {

        }
       
        void PathPainter::setPath(const QSFML::vector<sf::Vector2f>& path)
        {
			m_vertecies.clear();
			m_vertecies.resize(path.size() - 1);
			for (size_t i = 0; i < path.size() - 1; ++i)
			{
                m_vertecies[i].color = m_color;
                m_vertecies[i].position = path[i];
			}
        }
        void PathPainter::appenPoint(const sf::Vector2f& point)
        {
			m_vertecies.push_back({ point, m_color });
        }
        void PathPainter::popPointAtEnd(size_t count)
        {
			m_vertecies.erase(m_vertecies.end() - count, m_vertecies.end());
        }
        void PathPainter::popPointAtStart(size_t count)
        {
			m_vertecies.erase(m_vertecies.begin(), m_vertecies.begin() + count);
        }

		const sf::Vector2f& PathPainter::getPoint(size_t index) const
		{
			return m_vertecies[index].position;
		}

        void PathPainter::setColor(const sf::Color& color)
        {
            m_color = color;
            for (auto& line : m_vertecies)
            {
                line.color = color;
            }
        }
        void PathPainter::setColor(size_t index, const sf::Color& color)
        {
            m_vertecies[index].color = color;
        }
        const sf::Color& PathPainter::getColor() const
        {
            return m_color;
        }
        const sf::Color& PathPainter::getColor(size_t index) const
        {
            return m_vertecies[index].color;
        }
        void PathPainter::drawComponent(sf::RenderTarget& target,
                                        sf::RenderStates states) const
        {
#ifdef QSFML_USE_GL_DRAW
			QSFML_UNUSED(target);
            /*glLoadMatrixf(states.transform.getMatrix());
            glBegin(GL_LINE_STRIP);
			const sf::Vertex* data = m_vertecies.data();
            for (size_t i=0; i< m_vertecies.size(); ++i)
            {
				const auto& el = data[i];
                glColor4ub(el.color.r, el.color.g, el.color.b, el.color.a);
                glVertex2f(el.position.x, el.position.y);
            }
            glEnd();*/

            glLoadMatrixf(states.transform.getMatrix());

            glLineWidth(m_thickness);
            // Start drawing the line strip
            glBegin(GL_LINE_STRIP);

            // Retrieve the data pointer
            const sf::Vertex* data = m_vertecies.data();

            // Initialize previous color to a value that is different from any vertex color.
            sf::Color previousColor = sf::Color(0, 0, 0, 0);

            for (size_t i = 0; i < m_vertecies.size(); ++i)
            {
                const auto& el = data[i];

                // Only set the color if it's different from the previous vertex
                if (el.color != previousColor)
                {
                    glColor4ub(el.color.r, el.color.g, el.color.b, el.color.a);
                    previousColor = el.color;
                }

                // Set the vertex position
                glVertex2f(el.position.x, el.position.y);
            }

            glEnd();
#else
			target.draw(m_vertecies.data(), m_vertecies.size(), sf::LineStrip, states);
#endif
        }
    }
}
