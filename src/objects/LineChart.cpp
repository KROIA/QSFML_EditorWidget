#include "objects/LineChart.h"


namespace QSFML
{
namespace Objects
{

LineChart::LineChart(const std::string &name,
                     CanvasObject *parent)
    : CanvasObject(name, parent)
{
    setSize({10,10});
    m_yScale = 1;
    m_color = sf::Color::Green;
    m_autoScale = true;

    m_painter = new LineChartPainter();
    m_painter->m_chart = this;

    addComponent(m_painter);
}
LineChart::LineChart(const LineChart &other)
    : CanvasObject(other)
{
    m_size = other.m_size;
    m_origin = other.m_origin;
    m_dataPoints = other.m_dataPoints;
    m_color = other.m_color;
    m_yScale = other.m_yScale;
    m_autoScale = other.m_autoScale;

    m_painter = new LineChartPainter();
    m_painter->m_chart = this;
    addComponent(m_painter);
}
LineChart::~LineChart()
{

}

void LineChart::setOrigin(const sf::Vector2f &pos)
{
    m_origin = pos;
}
const sf::Vector2f &LineChart::getOrigin() const
{
    return m_origin;
}

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
void LineChart::enableAutoScale(bool enable)
{
    m_autoScale = enable;
}
bool LineChart::autoScaleEnabled() const
{
    return m_autoScale;
}
void LineChart::setScale(float yScale)
{
    m_autoScale = false;
    m_yScale = yScale;
}
float LineChart::getScale() const
{
    return m_yScale;
}

// void setXSpacing(float spacing);
// float getXSpacing() const;

void LineChart::setDataPoints(const std::vector<float> &dataPoints)
{
    m_dataPoints = dataPoints;
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
   /* static float x = 0;
    x += 0.05;
    m_dataPoints.push_back(sin(x));
    if(m_dataPoints.size() > 200)
        m_dataPoints.erase(m_dataPoints.begin());*/
    if(m_dataPoints.size() == 0)
       return;
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
void LineChart::LineChartPainter::draw(sf::RenderTarget& target,
                                       sf::RenderStates states) const
{
    if(!m_chart)
        return;

    // Draw axis
    sf::Vector2f origin = m_chart->m_origin;
    sf::Vector2f yTop = origin + sf::Vector2f(0, m_chart->m_size.y/2.f);
    sf::Vector2f yMin = origin + sf::Vector2f(0,-m_chart->m_size.y/2.f);
    sf::Vector2f xMax = origin + sf::Vector2f(m_chart->m_size.x, 0);
    sf::Vertex axis[]{
        sf::Vertex(yMin),
        sf::Vertex(yTop),
        sf::Vertex(origin),
        sf::Vertex(xMax),
    };
    target.draw(axis, 4, sf::Lines);

    // Draw content
    size_t count = m_chart->m_dataPoints.size();
    if(count < 2)
        return;
    float xSpacing = m_chart->m_size.x / ((float)count-1.f);
    sf::Vertex *vertecies = new sf::Vertex[count];

    float currentX = m_chart->m_origin.x;
    float yOffset = m_chart->m_origin.y;// - m_chart->m_size.y/2.f;
    for(size_t i=0; i<count; ++i)
    {
        vertecies[i].position = {currentX,
                                 -m_chart->m_dataPoints[i]*m_chart->m_yScale + yOffset};
        vertecies[i].color = m_chart->m_color;
        currentX += xSpacing;
    }

    target.draw(vertecies, count, sf::LineStrip);
    delete[] vertecies;
}
}
}
