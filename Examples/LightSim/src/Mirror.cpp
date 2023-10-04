#include "Mirror.h"


Mirror::Mirror(const std::string& name)
	: OpticalElement(name)
{
	m_mirrorLine = new QSFML::Components::Line();
	m_shape.m_width = 30;
	m_shape.m_angle = M_PI / 4;
	m_shape.m_pos = sf::Vector2f(0, 0);
	m_rotationSpeed = QSFML::Utilities::RandomEngine::getFloat(-M_PI / 100, M_PI / 100)*0.5;

	doesRefract(false);
	setShape(&m_shape);
	updateLine();
	addComponent(m_mirrorLine);
}
Mirror::Mirror(const Mirror& other)
	: OpticalElement(other)
{
	m_mirrorLine = new QSFML::Components::Line();
	m_shape = other.m_shape;
	
	m_rotationSpeed = other.m_rotationSpeed;
	setShape(&m_shape);
	updateLine();
	addComponent(m_mirrorLine);
}
Mirror::~Mirror()
{

}
OBJECT_IMPL(Mirror);

void Mirror::setColor(const sf::Color& c)
{
	m_mirrorLine->setColor(c);
}
const sf::Color& Mirror::getColor() const
{
	return m_mirrorLine->getColor();
}

void Mirror::setPos(const sf::Vector2f& pos)
{
	m_shape.m_pos = pos;
	updateLine();
}
const sf::Vector2f& Mirror::getPos() const
{
	return m_shape.m_pos;
}

void Mirror::setRotation(float angle)
{
	m_shape.m_angle = angle;
	updateLine();
}
float Mirror::getRotation() const
{
	return m_shape.m_angle;
}
void Mirror::setWidth(float w)
{
	m_shape.m_width = w;
	updateLine();
}
float Mirror::getWidth()
{
	return m_shape.m_width;
}

void Mirror::updateLine()
{
	sf::Vector2f direction = QSFML::VectorMath::getRotatedUnitVector(m_shape.m_angle);
	sf::Vector2f start = m_shape.m_pos - direction * m_shape.m_width / 2.f;
	sf::Vector2f end = m_shape.m_pos + direction * m_shape.m_width / 2.f;

	m_mirrorLine->setStartPos(start);
	m_mirrorLine->setEndPos(end);

	m_shape.m_mirrorRay.setPos(start);
	m_shape.m_mirrorRay.setDirection(direction * m_shape.m_width);
}

void Mirror::update()
{
	setRotation(getRotation() + m_rotationSpeed);
}

bool Mirror::MirrorShape::getCollisionData(const LightRay& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{
	float factorA, fac;

	if (m_mirrorRay.raycast(ray.ray, factorA, fac))
	{
		if (factorA >= 0 && factorA <= 1 && fac > 0.01)
		{
			outCollisionFactor = fac;
			//float rayAngle = ray.getAngle();
			//float mirrorAngle = m_mirrorRay.getAngle();

			//float angle 
			outNormalAngle = m_mirrorRay.getAngle() + M_PI_2;
			rayStartsInsideShape = false;
			return true;
		}
	}
	return false;
}
