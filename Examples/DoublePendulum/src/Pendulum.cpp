#include "Pendulum.h"

Pendulum::Pendulum(const std::string& name, CanvasObject* parent)
    : CanvasObject(name, parent)
{
    m_pointPainter = new QSFML::Components::PointPainter();
    m_pointPainter->setRadius(5);
    m_pointPainter->setColor(sf::Color(255, 100, 100));

    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line = new QSFML::Components::Line();
        m_pendulumData[i].line->setThickness(2);
        addComponent(m_pendulumData[i].line);
    }
    if (m_count > 1)
        m_pendulumData[1].angle = 0.3;
    
    addComponent(m_pointPainter);
    
    m_origin = sf::Vector2f(500, 0);
  
}
void Pendulum::update() 
{
    double dt = getDeltaT();
    std::vector<sf::Vector2f> points{
        m_origin
    };

    for (size_t i = 0; i < m_count; ++i)
    {
        PendulumData* prev = nullptr;
        if (i > 0)
            prev = &m_pendulumData[i-1];
        updatePendulum(m_pendulumData[i], prev, dt);
        points.push_back(m_pendulumData[i].endPos);
        m_pendulumData[i].line->setStartPos(m_pendulumData[i].startPos);
        m_pendulumData[i].line->setEndPos(m_pendulumData[i].endPos);
    }

    m_pointPainter->setPoints(points);   
}

void Pendulum::updatePendulum(PendulumData& pendulumData, PendulumData *prev, double dt)
{

    pendulumData.angleAcceleration = -m_gravity / pendulumData.length * sin(pendulumData.angle);
    pendulumData.angleVelocity = (pendulumData.angleVelocity + pendulumData.angleAcceleration * dt) *
                                 (1.0 - pendulumData.damping);
    pendulumData.angle += pendulumData.angleVelocity * dt;

    sf::Vector2f startPos = m_origin;
    if (prev)
        startPos = prev->endPos;
    pendulumData.startPos = startPos;
    pendulumData.endPos = startPos + (float)pendulumData.length * sf::Vector2f(sin(pendulumData.angle), cos(pendulumData.angle));
}