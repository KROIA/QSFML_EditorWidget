#include "components/drawable/LinePainter.h"
#include <math.h>
#include <SFML/Graphics.hpp>

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
void LinePainter::setPoints(const sf::Vector2f& start, const sf::Vector2f& end)
{
	m_lines = { {start, end, m_color, m_thickness} };
}
void LinePainter::setPoints(size_t index, const sf::Vector2f& start, const sf::Vector2f& end)
{
   LineData &data = m_lines[index];
   data.start = start;
   data.end = end;
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
	}
}
void LinePainter::setThickness(size_t index, float thickness)
{
    m_lines[index].thickness = thickness;
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
    if (m_useGlobalPosition)
        states = sf::RenderStates();
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
}
}
}
