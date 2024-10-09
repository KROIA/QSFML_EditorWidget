#include "objects/LineChart.h"
#include "components/drawable/Text.h"

namespace QSFML
{
namespace Objects
{
OBJECT_IMPL(LineChart)
LineChart::LineChart(const std::string &name,
                     GameObjectPtr parent)
    : GameObject(name, parent)
{
    setSize({10,10});
    m_yScale = 1;
    m_color = sf::Color::Green;
    m_autoScale = true;

    m_painter = new LineChartPainter();
    m_painter->m_chart = this;
    m_maxDataPoints = -1;

	m_valueText = new Components::Text("ValueText");
	m_valueText->setEnabled(false);
	m_valueText->setScale(0.1f);

    addComponent(m_painter);
	addComponent(m_valueText);
}
LineChart::LineChart(const LineChart &other)
    : GameObject(other)
{
    m_size = other.m_size;
   //m_origin = other.m_origin;
    m_dataPoints = other.m_dataPoints;
    m_color = other.m_color;
    m_yScale = other.m_yScale;
    m_autoScale = other.m_autoScale;
    m_maxDataPoints = other.m_maxDataPoints;

    m_painter = new  LineChartPainter();
    m_painter->m_chart = this;

    m_valueText = new Components::Text("ValueText");
    m_valueText->setEnabled(false);

    addComponent(m_painter);
    addComponent(m_valueText);
}
LineChart::~LineChart()
{

}
/*
void LineChart::setOrigin(const sf::Vector2f &pos)
{
    m_origin = pos;
}
const sf::Vector2f &LineChart::getOrigin() const
{
    return m_origin;
}
*/
void LineChart::setSize(const sf::Vector2f &size)
{
    m_size = size;
}
const sf::Vector2f &LineChart::getSize() const
{
    return m_size;
}
void LineChart::setColor(const sf::Color &color)
{
    m_color = color;
}
const sf::Color &LineChart::getColor() const
{
    return m_color;
}
void LineChart::setMaxDataPoints(size_t maxDataPoints)
{
    m_maxDataPoints = maxDataPoints;
}
size_t LineChart::getMaxDataPoints() const
{
    return m_maxDataPoints;
}
void LineChart::enableAutoScale(bool enable)
{
    m_autoScale = enable;
}
bool LineChart::autoScaleEnabled() const
{
    return m_autoScale;
}
void LineChart::setPlotScale(float yScale)
{
    m_autoScale = false;
    m_yScale = yScale;
}
float LineChart::getPlotScale() const
{
    return m_yScale;
}

void LineChart::addDataPoint(float value)
{
    m_dataPoints.push_back(value);
    if(m_dataPoints.size() > m_maxDataPoints)
		m_dataPoints.erase(m_dataPoints.begin());
}
void LineChart::removeFirstDataPoint()
{
    if(m_dataPoints.size() > 0)
    m_dataPoints.erase(m_dataPoints.begin());
}
void LineChart::setDataPoints(const std::vector<float> &dataPoints)
{
    m_dataPoints = dataPoints;
    if (m_dataPoints.size() > m_maxDataPoints)
        m_dataPoints.erase(m_dataPoints.begin(), m_dataPoints.end() - m_maxDataPoints);
}
const std::vector<float> &LineChart::getDataPoints() const
{
    return m_dataPoints;
}
void LineChart::clear()
{
    m_dataPoints.clear();
}

void LineChart::update()
{
    if(m_autoScale)
    {
        float min, max;
        getMinMax(min, max);
        float minSQ = abs(min);
        float maxSQ = abs(max);
        float divisor = maxSQ;

        if(minSQ > maxSQ)
            divisor = minSQ;
        if(divisor != 0)
        {
            m_yScale = m_size.y / (2*divisor);
        }
    }

	m_points.clear();
	m_points.reserve(m_dataPoints.size());
    size_t count = m_dataPoints.size();
    if (count < 2)
        return;
    float xSpacing = m_size.x / ((float)count - 1.f);

    float currentX = 0;
    float yOffset = 0;
    for (size_t i = 0; i < count; ++i)
    {
        m_points[i]= { currentX, -m_dataPoints[i] * m_yScale + yOffset };
        currentX += xSpacing;
    }

	sf::Vector2f mousePos = getMouseWorldPosition();
	sf::Vector2f pos = getGlobalPosition();
    if (mousePos.x >= pos.x && mousePos.x <= pos.x + currentX)
    {
		size_t index = (size_t)((mousePos.x - pos.x) / xSpacing);
		float yPos = m_points[index].y;
		if (mousePos.y >= yPos - 1 + pos.y && mousePos.y <= yPos + 1 + pos.y)
        {
            if (index < m_dataPoints.size())
            {
                m_valueText->setText(std::to_string(m_dataPoints[index]));
                m_valueText->setPosition({ mousePos.x, yPos });
                m_valueText->setEnabled(true);
            }
        }
		else
		{
			m_valueText->setEnabled(false);
		}
	}
    else
    {
        m_valueText->setEnabled(false);
    }
}
void LineChart::getMinMax(float &min, float &max)
{
    min = 99999;
    max = -99999;
    for(size_t i=0; i<m_dataPoints.size(); ++i)
    {
        if(m_dataPoints[i] < min)
            min = m_dataPoints[i];
        if(m_dataPoints[i] > max)
            max = m_dataPoints[i];
    }
}

LineChart::LineChartPainter::LineChartPainter(const std::string &name)
    : Drawable(name)
{
    m_chart = nullptr;
}
void LineChart::LineChartPainter::drawComponent(sf::RenderTarget& target,
                                       sf::RenderStates states) const
{
    QSFML_UNUSED(states);
    if(!m_chart)
        return;

    // Draw axis
    sf::Vector2f origin;// = m_chart->m_origin;
    sf::Vector2f yTop = origin + sf::Vector2f(0, m_chart->m_size.y/2.f);
    sf::Vector2f yMin = origin + sf::Vector2f(0,-m_chart->m_size.y/2.f);
    sf::Vector2f xMax = origin + sf::Vector2f(m_chart->m_size.x, 0);
    sf::Vertex axis[]{
        sf::Vertex(yMin),
        sf::Vertex(yTop),
        sf::Vertex(origin),
        sf::Vertex(xMax),
    };
    target.draw(axis, 4, sf::Lines, states);

    // Draw content
    size_t count = m_chart->m_dataPoints.size();
    if(count < 2)
        return;
//    float xSpacing = m_chart->m_size.x / ((float)count-1.f);
    sf::Vertex *vertecies = new sf::Vertex[count];

//    float currentX = origin.x;
//    float yOffset = origin.y;
    for(size_t i=0; i<count; ++i)
    {
        vertecies[i].position = m_chart->m_points[i];
        vertecies[i].color = m_chart->m_color;
    }

    target.draw(vertecies, count, sf::LineStrip, states);
    delete[] vertecies;
}
}
}
