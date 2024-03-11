#include "Pendulum.h"

Pendulum::Pendulum(const std::string& name, CanvasObject* parent)
    : QObject()
    , CanvasObject(name, parent)
{
    m_pointPainter = new QSFML::Components::PointPainter();
    m_pointPainter->setRadius(m_pendulumRadius);

    

    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line = new QSFML::Components::Line();
        m_pendulumData[i].line->setThickness(2);
        addComponent(m_pendulumData[i].line);
    }
    if (m_count > 1)
        m_pendulumData[1].angle = 0.0;
    
    addComponent(m_pointPainter);
    m_origin = sf::Vector2f(500, 500);
  

    m_mousePressEvent = new QSFML::Components::MousePressEvent();
    m_mousePressEvent->setTriggerButton(sf::Mouse::Button::Right);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::fallingEdge, this, &Pendulum::onMouseFalling);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::risingEdge, this, &Pendulum::onMouseRessing);
    addComponent(m_mousePressEvent);

    

   /* m_text = new QSFML::Components::Text();
    //m_text->setText("Pendulum");
	m_text->setCharacterSize(20);
    m_text->setScale(1);
    m_text->setOrigin(QSFML::Utilities::Origin::Center);
	m_text->setPosition(m_origin + sf::Vector2f(10,-40));
	addComponent(m_text);*/
}
void Pendulum::setStart(double angle1, double angle2)
{
	m_pendulumData[0].angle = angle1;
	m_pendulumData[1].angle = -angle2;

    int r = (1+sin(angle1)) * 127.5;
    int g = (1+sin(angle1+M_PI*2/3.f))*127.5;
    int b = (1+sin(angle1+M_PI*4/3.f))*127.5;
    m_pointPainter->setColor(sf::Color(r,g,b));
    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line->setColor(sf::Color(r,g,b));
        m_pendulumData[i].angleAcceleration = 0;
        m_pendulumData[i].angleVelocity = 0;
    }
}
void Pendulum::setLength(double length1, double length2)
{
	m_pendulumData[0].length = length1;
	m_pendulumData[1].length = length2;
}
void Pendulum::setLinesEnabled(bool enabled)
{
    m_linesEnabled = enabled;
	for (size_t i = 0; i < m_count; ++i)
	{
		m_pendulumData[i].line->setEnabled(enabled);
	}
}
void Pendulum::update() 
{
    double dt = getDeltaT();
    std::vector<sf::Vector2f> points{
       // m_origin
    };

    double acceleration1 = getAngleAcceleration1(m_pendulumData[0], m_pendulumData[1]);
    double acceleration2 = getAngleAcceleration2(m_pendulumData[0], m_pendulumData[1]);

    m_pendulumData[0].angleAcceleration = acceleration1;
    m_pendulumData[1].angleAcceleration = acceleration2;

    std::string text;
    double sumEnergy = 0;
    for (size_t i = 0; i < m_count; ++i)
    {
        PendulumData* prev = nullptr;
        sf::Vector2f startPos = m_origin;

        if (i > 0)
        {
            prev = &m_pendulumData[i - 1];
            startPos = prev->endPos;
        }
        updatePendulum(m_pendulumData[i], prev, dt);
		if (i == m_dragingIndex)
		{
			m_pendulumData[i].endPos = getMouseWorldPosition();
			m_pendulumData[i].angle = atan2(m_pendulumData[i].endPos.y - startPos.y, m_pendulumData[i].endPos.x - startPos.x);
	    }
        
        points.push_back(m_pendulumData[i].endPos);
        
        if (m_linesEnabled)
        {
            m_pendulumData[i].line->setStartPos(startPos);
            m_pendulumData[i].line->setEndPos(m_pendulumData[i].endPos);
        }

        double energy = getEnergy(m_pendulumData[i]);
        sumEnergy += energy;
        text += "E" + std::to_string(i) + ": " + std::to_string(energy) + "\n";
    }
    text += "SUM E: " + std::to_string(sumEnergy) + "\n";
    if(m_text)
        m_text->setText(text);

    m_pointPainter->setPoints(points);   
}

void Pendulum::updatePendulum(PendulumData& pendulumData, PendulumData *prev, double dt)
{
    //pendulumData.angleAcceleration = -m_gravity / pendulumData.length * sin(pendulumData.angle);
    pendulumData.angleVelocity = (pendulumData.angleVelocity + pendulumData.angleAcceleration * dt) *
                                 (1.0 - pendulumData.damping * dt);
    pendulumData.angle += pendulumData.angleVelocity * dt;

    sf::Vector2f startPos = m_origin;
    if (prev)
        startPos = prev->endPos;
    pendulumData.endPos = startPos + (float)pendulumData.length * sf::Vector2f(sin(pendulumData.angle), cos(pendulumData.angle));
}

double Pendulum::getAngleAcceleration1(const PendulumData& p1, const PendulumData& p2)
{
    double num1 = -m_gravity * (2 * p1.mass + p2.mass) * sin(p1.angle);
    double num2 = -p2.mass * m_gravity * sin(p1.angle - 2 * p2.angle);
    double num3 = -2 * sin(p1.angle - p2.angle) * p2.mass * 
                  (p2.angleVelocity * p2.angleVelocity * p2.length + p1.angleVelocity * p1.angleVelocity * p1.length * cos(p1.angle - p2.angle));
    double den = p1.length * (2 * p1.mass + p2.mass - p2.mass * cos(2 * p1.angle - 2 * p2.angle));

    return (num1 + num2 + num3) / den;
}
double Pendulum::getAngleAcceleration2(const PendulumData& p1, const PendulumData& p2)
{
    double num1 = 2 * sin(p1.angle - p2.angle);
	double num2 = (p1.angleVelocity * p1.angleVelocity * p1.length * (p1.mass + p2.mass));
	double num3 = m_gravity * (p1.mass + p2.mass) * cos(p1.angle);
	double num4 = p2.angleVelocity * p2.angleVelocity * p2.length * p2.mass * cos(p1.angle - p2.angle);
	double den = p2.length * (2 * p1.mass + p2.mass - p2.mass * cos(2 * p1.angle - 2 * p2.angle));

	return num1 * (num2 + num3 + num4) / den;
}
double Pendulum::getCineticEnergy(const PendulumData& p)
{
	return 0.5 * p.mass * p.length * p.length * p.angleVelocity * p.angleVelocity;
}
double Pendulum::getPotentialEnergy(const PendulumData& p)
{
	return m_gravity * p.mass * p.length * (1-cos(p.angle));
}
double Pendulum::getEnergy(const PendulumData& p)
{
    return getCineticEnergy(p) + getPotentialEnergy(p);
}


void Pendulum::onMouseFalling()
{
    sf::Vector2f mousePos = getMouseWorldPosition();
    double distance0 = QSFML::VectorMath::getLength(mousePos - m_pendulumData[0].endPos);
    double distance1 = QSFML::VectorMath::getLength(mousePos - m_pendulumData[0].endPos);

    if (distance0 < m_pendulumRadius*2)
    {
        m_dragingIndex = 0;
        return; 
    }
    if (distance1 < m_pendulumRadius*2)
    {
        m_dragingIndex = 1;
        return;
    }
    m_dragingIndex = -1;
}
void Pendulum::onMouseRessing()
{
    m_dragingIndex = -1;
}