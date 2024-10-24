#include "Pendulum.h"
#define _USE_MATH_DEFINES
#include <math.h>

Pendulum::Pendulum(const std::string& name, QSFML::Objects::GameObjectPtr parent)
    : QObject()
    , GameObject(name, parent)
{
    m_pointPainter = new QSFML::Components::PointPainter();
    m_pointPainter->setRadius(m_pendulumRadius);
	m_pathPainter = new QSFML::Components::PathPainter();
	

	
    

    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line = new QSFML::Components::LinePainter();
        m_pendulumData[i].line->setThickness(2);
        addComponent(m_pendulumData[i].line);
    }
    if (m_count > 1)
        m_pendulumData[1].angle = 0.0;
    
    addComponent(m_pointPainter);
    m_origin = QSFML::VectorMath::Vector2d(0, 0);
  

    m_mousePressEvent = new QSFML::Components::MousePressEvent();
    m_mousePressEvent->setTriggerButton(sf::Mouse::Button::Right);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::fallingEdge, this, &Pendulum::onMouseFalling);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::risingEdge, this, &Pendulum::onMouseRessing);
    addComponent(m_mousePressEvent);

    
    m_text = nullptr;
    m_text = new QSFML::Components::Text();
    //m_text->setText("Pendulum");
	m_text->setCharacterSize(20);
    m_text->setScale(1);
    m_text->setOrigin(QSFML::Utilities::Origin::Center);
	
	addComponent(m_text);
    addComponent(m_pathPainter);
    enableText(false);
    
    //m_chart = new QSFML::Objects::LineChart();
    //m_chart->setOrigin();
   // m_chart->setPositionRelative(sf::Vector2f(0, -50));
}
void Pendulum::setStart(double angle1, double angle2)
{
	m_pendulumData[0].angle = angle1;
	m_pendulumData[1].angle = angle2;

    
    for (size_t i = 0; i < m_count; ++i)
    {
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
        m_text->setPosition(sf::Vector2f(10, -40));
    //m_origin = QSFML::VectorMath::Vector2d(getPosition().x, getPosition().y);
}
void Pendulum::setLength(double length1, double length2)
{
	m_pendulumData[0].length = length1;
	m_pendulumData[1].length = length2;
}
void Pendulum::setDamping(float damping1, float damping2)
{
    m_pendulumData[0].damping = damping1;
    m_pendulumData[1].damping = damping2;
}
void Pendulum::setLinesEnabled(bool enabled)
{
    m_linesEnabled = enabled;
	for (size_t i = 0; i < m_count; ++i)
	{
		m_pendulumData[i].line->setEnabled(enabled);
	}
}
void Pendulum::setColor(const sf::Color& color)
{
    for (size_t i = 0; i < m_count; ++i)
    {
        m_pendulumData[i].line->setColor(color);
    }
    m_pointPainter->setColor(color);
	m_pathPainter->setColor(color);
}

void Pendulum::setMaxPathLength(size_t length)
{
    m_maxPathLength = length;
    if (m_pathPainter->getPointCount() > length)
		m_pathPainter->popPointAtStart(m_pathPainter->getPointCount() - length);
}

void Pendulum::enableText(bool enabled)
{
    m_text->setEnabled(enabled);
}
void Pendulum::enablePath(bool enabled)
{
	m_pathPainter->setEnabled(enabled);
}
void Pendulum::enableEnergyCorrection(bool enabled)
{
    m_energyCorrectionEnabled = enabled;
}
void Pendulum::update() 
{
    double dt = getFixedDeltaT();
    //double dt = getDeltaT();
    QSFML::vector<sf::Vector2f> points{
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
                sf::Vector2f dir = getMouseWorldPosition() - (sf::Vector2f(startPos.x, startPos.y) + getPosition());
                m_pendulumData[i].angle = QSFML::VectorMath::getAngleRAD(dir) - (float)M_PI_2;
                m_pendulumData[i].angleVelocity = 0;
                m_pendulumData[i].angleAcceleration = 0;
            }
            updatePendulum(m_pendulumData[i], prev, dt);
            
            
            
            
            double ePot = getPotentialEnergy(m_pendulumData[i]);
            double eKin = getKineticEnergy(m_pendulumData[i], dt);
            sumEnergy += (ePot + eKin);

            text += "E_Pot" + std::to_string(i) + ": " + std::to_string(ePot) + "\n";
            text += "E_Kin" + std::to_string(i) + ": " + std::to_string(eKin) + "\n";

			
        }

        if (m_pathPainter->getPointCount() > m_maxPathLength)
            m_pathPainter->popPointAtStart(m_pathPainter->getPointCount() - m_maxPathLength);
        m_pathPainter->appenPoint(sf::Vector2f(m_pendulumData[1].endPos.x, m_pendulumData[1].endPos.y));

   // }

    text += "SUM E: " + std::to_string(sumEnergy) + "\n";
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
            m_pendulumData[i].line->setPoints(sf::Vector2f(startPos.x, startPos.y), sf::Vector2f(m_pendulumData[i].endPos.x, m_pendulumData[i].endPos.y));
        }
    }
    m_pointPainter->setPoints(points);   

    if (m_energyCorrectionEnabled)
    {
        applyEnergyCorrection(m_pendulumData[0], m_pendulumData[1], 0, dt);
    }
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
    pendulumData.endPos = startPos + (double)pendulumData.length * QSFML::VectorMath::getRotatedUnitVectorRAD((double)pendulumData.angle + (float)M_PI_2);
}
void Pendulum::applyEnergyCorrection(PendulumData& p1, PendulumData& p2, double targetEnergy, double dt)
{
    if (dt <= 0.00000001)
        return;
    double ePot1 = getPotentialEnergy(p1);
    double ePot2 = getPotentialEnergy(p2);

    double eKin1 = getKineticEnergy(p1, dt);
    double eKin2 = getKineticEnergy(p2, dt);

    double eSum = ePot1 + ePot2 + eKin1 + eKin2;
    double eDiff = targetEnergy - eSum;

    double eDiffKin1 = eKin1;
    double eDiffKin2 = eKin2;

    double newVel1 = sqrt(2 * eDiffKin1 / p1.mass) / p1.length;
    double newVel2 = sqrt(2 * eDiffKin2 / p2.mass) / p2.length;
    if(p1.angleVelocity < 0)
        p1.angleVelocity = -newVel1;
    else
        p1.angleVelocity = newVel1;
    
    if(p2.angleVelocity < 0)
        p2.angleVelocity = -newVel2;
    else
        p2.angleVelocity = newVel2;

    
}

double Pendulum::getAngleAcceleration1(const PendulumData& p1, const PendulumData& p2)
{
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
    /*
    double lm = -m2 / (m1 + m2) * l2 / l1;
    double sum1 = a2 * cos(phi1 - phi2);
    double sum2 = v2 * v2 * sin(phi1 - phi2);
    double sum3 = -g / l1 * sin(phi1);

    double acc = lm * (sum1 + sum2) + sum3;
    return acc;*/

    double num1 = -g * (2 * m1 + m2) * sin(phi1);
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

    double l = -l1 / l2;
    double sum1 = a1 * cos(phi1 - phi2);
    double sum2 = -v1 * v1 * sin(phi1 - phi2);
    double sum3 = -g / l2 * sin(phi2);
    
    double acc = l * (sum1 + sum2) + sum3;
    return acc;*/
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
    double height = - p.endPos.y;
    return m_gravity * p.mass * height;
}
double Pendulum::getEnergy(const PendulumData& p, double dt)
{
    return getKineticEnergy(p, dt) + getPotentialEnergy(p);
}


void Pendulum::onMouseFalling()
{
    QSFML::VectorMath::Vector2d mousePos = QSFML::VectorMath::Vector2d(getMouseWorldPosition().x, getMouseWorldPosition().y);
    QSFML::VectorMath::Vector2d objPos(getPosition());
    double distance0 = QSFML::VectorMath::getLength(mousePos - (m_pendulumData[0].endPos+objPos));
    double distance1 = QSFML::VectorMath::getLength(mousePos - (m_pendulumData[1].endPos+objPos));

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
