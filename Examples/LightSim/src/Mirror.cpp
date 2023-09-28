#include "Mirror.h"


Mirror::Mirror(const std::string& name)
	: OpticalElement(name)
{
	m_mirrorLine = new QSFML::Components::Line();
	m_width = 30;
	m_angle = M_PI / 4;
	m_pos = sf::Vector2f(0, 0);
	m_rotationSpeed = QSFML::Utilities::RandomEngine::getFloat(-M_PI / 100, M_PI / 100)*0.5;
	updateLine();
	addComponent(m_mirrorLine);
}
Mirror::Mirror(const Mirror& other)
	: OpticalElement(other)
{
	m_mirrorLine = new QSFML::Components::Line();
	m_width = other.m_width;
	m_angle = other.m_angle;
	m_pos = other.m_pos;
	m_mirrorRay = other.m_mirrorRay;
	m_rotationSpeed = other.m_rotationSpeed;
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
	m_pos = pos;
	updateLine();
}
const sf::Vector2f& Mirror::getPos() const
{
	return m_pos;
}

void Mirror::setRotation(float angle)
{
	m_angle = angle;
	updateLine();
}
float Mirror::getRotation() const
{
	return m_angle;
}
void Mirror::setWidth(float w)
{
	m_width = w;
	updateLine();
}
float Mirror::getWidth()
{
	return m_width;
}

void Mirror::updateLine()
{
	sf::Vector2f direction = QSFML::VectorMath::getRotatedUnitVector(m_angle);
	sf::Vector2f start = m_pos - direction * m_width / 2.f;
	sf::Vector2f end = m_pos + direction * m_width / 2.f;

	m_mirrorLine->setStartPos(start);
	m_mirrorLine->setEndPos(end);

	m_mirrorRay.setPos(start);
	m_mirrorRay.setDirection(direction * m_width);
}

bool Mirror::processLaser(const QSFML::Utilities::Ray& ray, 
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	float factorA, factorB;
	if (m_mirrorRay.raycast(ray, factorA, factorB))
	{
		if (factorA < 0 || factorA > 1)
			return false;
		sf::Vector2f collisionPoint = m_mirrorRay.getPoint(factorA);

		float rayAngle = QSFML::VectorMath::getAngle(ray.getDirection());
		float newAngle = 2 * m_angle - rayAngle;


		//qDebug() << "new Angle: " << newAngle*180/M_PI << " " << rayAngle;

		QSFML::Utilities::Ray reflected(collisionPoint, QSFML::VectorMath::getRotatedUnitVector(newAngle));
		reflectedOut.push_back(reflected);
		return true;
	}
	return false;
}
bool Mirror::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
	float factorA;

	if (m_mirrorRay.raycast(ray, factorA, distanceOut))
	{
		if (factorA >= 0 && factorA <= 1 && distanceOut > 0.1)
			return true;
	}
	return false;
}

void Mirror::update()
{
	//setRotation(getRotation() + m_rotationSpeed);
}