#include "components/drawable/LinePainter.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(LinePainter)
LinePainter::LinePainter(const std::string &name)
    : Drawable(name)
    , m_thickness(1)
    , m_color(sf::Color::Green)
{

}
LinePainter::LinePainter(const LinePainter &other)
    : Drawable(other)
    , m_thickness(other.m_thickness)
    , m_color(other.m_color)
    , m_lines(other.m_lines)
{

}

void LinePainter::LineData::updateGeometry()
{
    sf::Vector2f direction = end - start;
    float dist = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (dist == 0)
    {
        points[0] = start;
        points[1] = start;
        points[2] = start;
        points[3] = start;
		return;
    }
    sf::Vector2f unitDirection = direction / dist;
    sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);
    sf::Vector2f offset = (thickness * 0.5f) * unitPerpendicular;

    points[0] = start + offset;
    points[1] = end + offset;
    points[2] = end - offset;
    points[3] = start - offset;
}

void LinePainter::setPoints(const sf::Vector2f& start, const sf::Vector2f& end)
{
	m_lines = { {start, end, m_color, m_thickness} };
	m_lines[0].updateGeometry();
}
void LinePainter::setPoints(size_t index, const sf::Vector2f& start, const sf::Vector2f& end)
{
   LineData &data = m_lines[index];
   data.start = start;
   data.end = end;
   data.updateGeometry();
}
void LinePainter::addLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& col, float thickness)
{
    m_lines.push_back({ start, end, col, thickness });
	m_lines.back().updateGeometry();
}
void LinePainter::addLine(const sf::Vector2f& start, const sf::Vector2f& end)
{
    m_lines.push_back({ start, end, m_color, m_thickness });
	m_lines.back().updateGeometry();
}
const sf::Vector2f &LinePainter::getStartPos() const
{
    return m_lines[0].start;
}
const sf::Vector2f& LinePainter::getStartPos(size_t index) const
{
    return m_lines[index].start;
}
const sf::Vector2f &LinePainter::getEndPos() const
{
    return m_lines[0].end;
}
const sf::Vector2f& LinePainter::getEndPos(size_t index) const
{
    return m_lines[index].end;
}
void LinePainter::setThickness(float thickness)
{
    m_thickness = thickness;
    for (auto& line : m_lines)
    {
		line.thickness = thickness;
		line.updateGeometry();
	}
}
void LinePainter::setThickness(size_t index, float thickness)
{
    m_lines[index].thickness = thickness;
	m_lines[index].updateGeometry();
}
float LinePainter::getThickness() const
{
    return m_thickness;
}
float LinePainter::getThickness(size_t index) const
{
    return m_lines[index].thickness;
}
void LinePainter::setColor(const sf::Color &color)
{
    m_color = color;
    for (auto& line : m_lines)
    {
        line.color = color;
    }
}
void LinePainter::setColor(size_t index, const sf::Color& color)
{
    m_lines[index].color = color;
}
const sf::Color &LinePainter::getColor() const
{
    return m_color;
}
const sf::Color& LinePainter::getColor(size_t index) const
{
    return m_lines[index].color;
}
void LinePainter::drawComponent(sf::RenderTarget& target,
                        sf::RenderStates states) const
{
#ifdef QSFML_USE_GL_DRAW
    QSFML_UNUSED(target);
    glLoadMatrixf(states.transform.getMatrix());

    // Enable blending to match SFML's alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Iterate through the lines and draw each one using OpenGL
    for (auto& line : m_lines)
    {
        // Set the color for the line
        sf::Color color = line.color;

        // Draw the quad using OpenGL immediate mode
        glBegin(GL_QUADS);
        glColor4ub(color.r, color.g, color.b, color.a); // Set color with RGBA
        glVertex2f(line.points[0].x, line.points[0].y);
        glVertex2f(line.points[1].x, line.points[1].y);
        glVertex2f(line.points[2].x, line.points[2].y);
        glVertex2f(line.points[3].x, line.points[3].y);
        glEnd();
    }
#else    
    for (auto& line : m_lines)
    {
        sf::Vertex m_relativeVertices[4];
        sf::Vector2f direction = line.end - line.start;
        float dist =  sqrt(direction.x*direction.x+direction.y*direction.y);
        if(dist == 0)
			continue;
		sf::Vector2f unitDirection = direction / dist;
		sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

		sf::Vector2f offset = (line.thickness/2.f)*unitPerpendicular;

		m_relativeVertices[0].position = line.start + offset;
		m_relativeVertices[1].position = line.end + offset;
		m_relativeVertices[2].position = line.end - offset;
		m_relativeVertices[3].position = line.start - offset;
		for (int i=0; i<4; ++i)
			m_relativeVertices[i].color = line.color;
		target.draw(m_relativeVertices,4,sf::Quads, states);
    }
#endif
}
}
}
