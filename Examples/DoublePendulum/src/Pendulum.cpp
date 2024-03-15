#include "Pendulum.h"
#define _USE_MATH_DEFINES
#include <math.h>

Pendulum::Pendulum(const std::string& name, CanvasObject* parent)
    : QObject()
    , CanvasObject(name, parent)
{
    m_pointPainter = new QSFML::Components::PointPainter();
    m_pointPainter->setRadius(m_pendulumRadius);

    

    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line = new QSFML::Components::LinePainter();
        m_pendulumData[i].line->setThickness(2);
        addComponent(m_pendulumData[i].line);
    }
    if (m_count > 1)
        m_pendulumData[1].angle = 0.0;
    
    addComponent(m_pointPainter);
    //m_origin = sf::Vector2f(0, 0);
  

    m_mousePressEvent = new QSFML::Components::MousePressEvent();
    m_mousePressEvent->setTriggerButton(sf::Mouse::Button::Right);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::fallingEdge, this, &Pendulum::onMouseFalling);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::risingEdge, this, &Pendulum::onMouseRessing);
    addComponent(m_mousePressEvent);

    
    m_text = nullptr;
    /*m_text = new QSFML::Components::Text();
    //m_text->setText("Pendulum");
	m_text->setCharacterSize(20);
    m_text->setScale(1);
    m_text->setOrigin(QSFML::Utilities::Origin::Center);
	
	addComponent(m_text);
    */
    //m_chart = new QSFML::Objects::LineChart();
    //m_chart->setOrigin();
   // m_chart->setPositionRelative(sf::Vector2f(0, -50));
}
void Pendulum::setStart(double angle1, double angle2)
{
	m_pendulumData[0].angle = angle1;
	m_pendulumData[1].angle = angle2;

    int r = (1+sin(angle1)) * 127.5;
    int g = (1+sin(angle1+M_PI*2/3.f))*127.5;
    int b = (1+sin(angle1+M_PI*4/3.f))*127.5;
    m_pointPainter->setColor(sf::Color(r,g,b));
    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line->setColor(sf::Color(r,g,b));
        m_pendulumData[i].angleAcceleration = 0;
        m_pendulumData[i].angleVelocity = 0;
        PendulumData* prev = nullptr;
        

        if (i > 0)
        {
            prev = &m_pendulumData[i - 1];
        }
        updatePendulum(m_pendulumData[i], prev, 0);
    }
    if(m_text)
        m_text->setPosition(getPositionAbsolute() + sf::Vector2f(10, -40));
    m_origin = QSFML::VectorMath::Vector2d(getPositionAbsolute().x, getPositionAbsolute().y);
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
    double dt = getFixedDeltaT();
    std::vector<sf::Vector2f> points{
       // m_origin
    };
    //int substeps = 1000;
    //dt /= (double)substeps;
    double sumEnergy = 0;
    std::string text;
    //for (int j = 0; j < substeps; ++j)
   // {

        double acceleration1 = getAngleAcceleration1(m_pendulumData[0], m_pendulumData[1]);
        double acceleration2 = getAngleAcceleration2(m_pendulumData[0], m_pendulumData[1]);

        m_pendulumData[0].angleAcceleration = acceleration1;
        m_pendulumData[1].angleAcceleration = acceleration2;

        
        
        for (size_t i = 0; i < m_count; ++i)
        {
            PendulumData* prev = nullptr;
            QSFML::VectorMath::Vector2d startPos = m_origin;

            if (i > 0)
            {
                prev = &m_pendulumData[i - 1];
                startPos = prev->endPos;
            }
            m_pendulumData[i].lastEndPos = m_pendulumData[i].endPos;
            if (i == m_dragingIndex)
            {
                //m_pendulumData[i].endPos = getMouseWorldPosition();
                sf::Vector2f dir = getMouseWorldPosition() - sf::Vector2f(startPos.x, startPos.y);
                m_pendulumData[i].angle = QSFML::VectorMath::getAngle(dir) - (float)M_PI_2;
                m_pendulumData[i].angleVelocity = 0;
                m_pendulumData[i].angleAcceleration = 0;
            }
            updatePendulum(m_pendulumData[i], prev, dt);
            
            
            /*
            
            double ePot = getPotentialEnergy(m_pendulumData[i]);
            double eKin = getKineticEnergy(m_pendulumData[i], dt);
            sumEnergy += (ePot + eKin);

            text += "E_Pot" + std::to_string(i) + ": " + std::to_string(ePot) + "\n";
            text += "E_Kin" + std::to_string(i) + ": " + std::to_string(eKin) + "\n";
            */
        }
   // }

   // text += "SUM E: " + std::to_string(sumEnergy) + "\n";
    if(m_text)
        m_text->setText(text);
    for (size_t i = 0; i < m_count; ++i)
    {
        points.push_back(sf::Vector2f(m_pendulumData[i].endPos.x, m_pendulumData[i].endPos.y));
        QSFML::VectorMath::Vector2d startPos = m_origin;

        if (i > 0)
        {
            startPos = m_pendulumData[i - 1].endPos;
        }
        if (m_linesEnabled)
        {
            m_pendulumData[i].line->setPos(sf::Vector2f(startPos.x, startPos.y), sf::Vector2f(m_pendulumData[i].endPos.x, m_pendulumData[i].endPos.y));
        }
    }
    m_pointPainter->setPoints(points);   
    //m_chart->setDataPoints(m_chartData);
}

void Pendulum::updatePendulum(PendulumData& pendulumData, PendulumData *prev, double dt)
{
    //pendulumData.angleAcceleration = 0.0;
    //pendulumData.angleAcceleration = -m_gravity / pendulumData.length * sin(pendulumData.angle);
    pendulumData.angleVelocity = (pendulumData.angleVelocity + pendulumData.angleAcceleration * dt) *
                                 (1.0 - pendulumData.damping * dt);
    //pendulumData.angleVelocity = 1;
    pendulumData.angle += pendulumData.angleVelocity * dt;

    QSFML::VectorMath::Vector2d startPos = m_origin;
    if (prev)
        startPos = prev->endPos;
    pendulumData.endPos = startPos + (double)pendulumData.length * QSFML::VectorMath::getRotatedUnitVector((double)pendulumData.angle + (float)M_PI_2);
}

double Pendulum::getAngleAcceleration1(const PendulumData& p1, const PendulumData& p2)
{
    /*double num1 = -m_gravity * (2 * p1.mass + p2.mass) * sin(p1.angle);
    double num2 = -p2.mass * m_gravity * sin(p1.angle - 2 * p2.angle);
    double num3 = -2 * sin(p1.angle - p2.angle) * p2.mass * 
                  (p2.angleVelocity * p2.angleVelocity * p2.length + p1.angleVelocity * p1.angleVelocity * p1.length * cos(p1.angle - p2.angle));
    double den = p1.length * (2 * p1.mass + p2.mass - p2.mass * cos(2 * p1.angle - 2 * p2.angle));

    return (num1 + num2 + num3) / den;*/
    double m1 = p1.mass;
    double m2 = p2.mass;
    double l1 = p1.length;
    double l2 = p2.length;
    double g = m_gravity;
    double a1 = p1.angleAcceleration;
    double a2 = p2.angleAcceleration;
    double v1 = p1.angleVelocity;
    double v2 = p2.angleVelocity;
    double phi1 = p1.angle;
    double phi2 = p2.angle;

    double num1 = -g * (2 * m1 + m2)* sin(phi1);
    double num2 = -m2 * g * sin(phi1 - 2 * phi2);
    double num3 = -2 * sin(phi1 - phi2) * m2 * (v2 * v2 * l2 + v1 * v1 * l1 * cos(phi1 - phi2));
    double den = l1 * (2 * m1 + m2 - m2 * cos(2 * phi1 - 2 * phi2));

    double a = (num1+num2+num3)/den;
    return a;
}
double Pendulum::getAngleAcceleration2(const PendulumData& p1, const PendulumData& p2)
{
    double num1 = 2 * sin(p1.angle - p2.angle);
	double num2 = (p1.angleVelocity * p1.angleVelocity * p1.length * (p1.mass + p2.mass));
	double num3 = m_gravity * (p1.mass + p2.mass) * cos(p1.angle);
	double num4 = p2.angleVelocity * p2.angleVelocity * p2.length * p2.mass * cos(p1.angle - p2.angle);
	double den = p2.length * (2 * p1.mass + p2.mass - p2.mass * cos(2 * p1.angle - 2 * p2.angle));

	return num1 * (num2 + num3 + num4) / den;
    /*double m1 = p1.mass;
    double m2 = p2.mass;
    double l1 = p1.length;
    double l2 = p2.length;
    double g = m_gravity;
    double a1 = p1.angleAcceleration;
    double a2 = p2.angleAcceleration;
    double v1 = p1.angleVelocity;
    double v2 = p2.angleVelocity;
    double phi1 = p1.angle;
    double phi2 = p2.angle;

    double num2 = v1 * v1 * sin(phi1 - phi2);
    double num1 = -l1 / l2 * (a1 * cos(phi1 - phi2) - num2);
    double num3 = -g / l2 * sin(phi2);
    double a = num1 + num3;
    return a;*/
}
double Pendulum::getKineticEnergy(const PendulumData& p, double dt)
{
    QSFML::VectorMath::Vector2d dv = p.endPos - p.lastEndPos;
    double vdt = QSFML::VectorMath::getLength(dv);
    if(vdt < 0.0001)
		return 0;
    double v = vdt / dt;
    //qDebug() << v;
    return 0.5 * p.mass * v * v;
}
double Pendulum::getPotentialEnergy(const PendulumData& p)
{
    double height = getPositionAbsolute().y - p.endPos.y;
    return m_gravity * p.mass * height;
}
double Pendulum::getEnergy(const PendulumData& p, double dt)
{
    return getKineticEnergy(p, dt) + getPotentialEnergy(p);
}


void Pendulum::onMouseFalling()
{
    QSFML::VectorMath::Vector2d mousePos = QSFML::VectorMath::Vector2d(getMouseWorldPosition().x, getMouseWorldPosition().y);
    double distance0 = QSFML::VectorMath::getLength(mousePos - m_pendulumData[0].endPos);
    double distance1 = QSFML::VectorMath::getLength(mousePos - m_pendulumData[1].endPos);

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
