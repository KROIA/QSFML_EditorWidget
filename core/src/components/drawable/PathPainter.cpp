#include "components/drawable/PathPainter.h"
#include "utilities/Color.h"
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
			, m_enableFadeColor(false)
			, m_fadeColors({ sf::Color::Green, sf::Color::Red })
        {

        }
        PathPainter::PathPainter(const PathPainter& other)
            : Drawable(other)
            , m_thickness(other.m_thickness)
            , m_color(other.m_color)
            , m_vertecies(other.m_vertecies)
			, m_enableFadeColor(other.m_enableFadeColor)
			, m_fadeColors(other.m_fadeColors)
        {

        }
       
        void PathPainter::setPath(const QSFML::vector<sf::Vector2f>& path)
        {
			m_vertecies.clear();
			m_vertecies.resize(path.size());
			for (size_t i = 0; i < path.size(); ++i)
			{
                m_vertecies[i].color = m_color;
                m_vertecies[i].position = path[i];
			}
        }
        QSFML::vector<sf::Vector2f> PathPainter::getPath() const
        {
            QSFML::vector<sf::Vector2f> p;
            p.reserve(m_vertecies.size());
            for (size_t i = 0; i < m_vertecies.size(); ++i)
            {
                p.push_back(m_vertecies[i].position);
            }
            return p;
        }
        void PathPainter::appendPoint(const sf::Vector2f& point)
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
            glLineWidth(m_thickness);
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

            
            // Start drawing the line strip
            glBegin(GL_LINE_STRIP);

            // Retrieve the data pointer
            const sf::Vertex* data = m_vertecies.data();

            if (m_enableFadeColor)
            {
                for (size_t i = 0; i < m_vertecies.size(); ++i)
                {
                    const auto& el = data[i];

                    // Only set the color if it's different from the previous vertex
					sf::Color color = Color::lerpLinear(m_fadeColors, i / static_cast<float>(m_vertecies.size()));
                    glColor4ub(color.r, color.g, color.b, color.a);


                    // Set the vertex position
                    glVertex2f(el.position.x, el.position.y);
                }
            }
            else
            {
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
            }
            
            glEnd();
            
#else
            if (m_enableFadeColor)
            {
				for (size_t i = 0; i < m_vertecies.size(); ++i)
				{
					m_vertecies[i].color = Color::lerpLinear(m_fadeColors, i / static_cast<float>(m_vertecies.size()));
				}
		    }
			target.draw(m_vertecies.data(), m_vertecies.size(), sf::LineStrip, states);
#endif
            glLineWidth(1);
        }
    }
}
