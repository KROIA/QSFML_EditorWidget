#include "CurvedMirror.h"
#define _USE_MATH_DEFINES
#include <math.h>

CurvedMirror::CurvedMirror(unsigned int resolution, const std::string& name)
	: OpticalElement(name)
{
	for (size_t i = 0; i < resolution; ++i)
	{
		QSFML::Components::Line* line = new QSFML::Components::Line();
		m_mirrorLines.push_back(line);
		m_shape.m_mirrorRays.push_back(QSFML::Utilities::Ray());
		addComponent(line);
	}
	m_shape.m_resolution = resolution;
	m_shape.m_openingAngle = M_PI / 4;
	m_shape.m_radius = 40;
	m_shape.m_angle = M_PI / 4;
	m_shape.m_pos = sf::Vector2f(0, 0);
	m_rotationSpeed = QSFML::Utilities::RandomEngine::getFloat(-M_PI / 100, M_PI / 100) *0.5;

	//setRefractionIndexOutside(1);
	//setRefractionIndexInside(-1);
	doesRefract(false);
	setShape(&m_shape);
	updateLine();
	
}
CurvedMirror::CurvedMirror(const CurvedMirror& other)
	: OpticalElement(other)
{
	
	
	m_shape = other.m_shape;
	for (size_t i = 0; i < m_shape.m_resolution; ++i)
	{
		QSFML::Components::Line* line = new QSFML::Components::Line();
		m_mirrorLines.push_back(line);
		m_shape.m_mirrorRays.push_back(QSFML::Utilities::Ray());
		addComponent(line);
	}
	m_rotationSpeed = other.m_rotationSpeed;
	setShape(&m_shape);
	updateLine();
}
CurvedMirror::~CurvedMirror()
{

}
OBJECT_IMPL(CurvedMirror);

void CurvedMirror::setColor(const sf::Color& c)
{
	for (size_t i = 0; i < m_mirrorLines.size(); ++i)
	{
		m_mirrorLines[i]->setColor(c);
	}
}
const sf::Color& CurvedMirror::getColor() const
{
	if (m_mirrorLines.size() == 0)
		return sf::Color::Black;
	return m_mirrorLines[0]->getColor();
}

void CurvedMirror::setPos(const sf::Vector2f& pos)
{
	m_shape.m_pos = pos;
	updateLine();
}
const sf::Vector2f& CurvedMirror::getPos() const
{
	return m_shape.m_pos;
}

void CurvedMirror::setRotation(float angle)
{
	m_shape.m_angle = angle;
	updateLine();
}
float CurvedMirror::getRotation() const
{
	return m_shape.m_angle;
}
void CurvedMirror::setOpeningAngle(float angle)
{
	m_shape.m_openingAngle = angle;
	updateLine();
}
float CurvedMirror::getOpeningAngle() const
{
	return m_shape.m_openingAngle;
}

void CurvedMirror::setRadius(float r)
{
	m_shape.m_radius = r;
	updateLine();
}
float CurvedMirror::getRadius() const
{
	return m_shape.m_radius;
}

void CurvedMirror::updateLine()
{
	float dAlpha = m_shape.m_openingAngle / m_shape.m_resolution;
	float alpha = -m_shape.m_openingAngle/2;
	sf::Vector2f start = QSFML::VectorMath::getRotatedUnitVector(alpha) * m_shape.m_radius;
	sf::Vector2f rotationPoint = QSFML::VectorMath::getUnitVector() * m_shape.m_radius;
	start = QSFML::VectorMath::getRotated(start, rotationPoint, m_shape.m_angle) + m_shape.m_pos;
	for (unsigned int i = 0; i < m_shape.m_resolution; ++i)
	{
		alpha += dAlpha;
		sf::Vector2f end = QSFML::VectorMath::getRotatedUnitVector(alpha) * m_shape.m_radius;
		end = QSFML::VectorMath::getRotated(end, rotationPoint, m_shape.m_angle) + m_shape.m_pos;
		m_mirrorLines[i]->setStartPos(start);
		m_mirrorLines[i]->setEndPos(end);
		m_shape.m_mirrorRays[i].setPos(start);
		m_shape.m_mirrorRays[i].setDirection(end - start);
		start = end;
	}

	/*m_CurvedMirrorLine->setStartPos(start);
	m_CurvedMirrorLine->setEndPos(end);

	m_CurvedMirrorRay.setPos(start);
	m_CurvedMirrorRay.setDirection(direction * m_width);*/
}
/*
bool CurvedMirror::processLaser(const LightRay& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	float factorA, factorB;

	size_t i;
	if (getShortestDistanceAndIndex(ray, factorA, factorB, i))
	{
		sf::Vector2f collisionPoint = m_mirrorRays[i].getPoint(factorA);
		float subshapeAngle = QSFML::VectorMath::getAngle(m_mirrorRays[i].getDirection());

		float rayAngle = QSFML::VectorMath::getAngle(ray.getDirection());
		float newAngle = 2 * subshapeAngle - rayAngle;


		//qDebug() << "new Angle: " << newAngle*180/M_PI << " " << rayAngle;

		QSFML::Utilities::Ray reflected(collisionPoint, QSFML::VectorMath::getRotatedUnitVector(newAngle));
		reflectedOut.push_back(reflected);
		return true;
	}
	return false;
}
bool CurvedMirror::getRaycastDistance(const LightRay& ray, float& distanceOut) const
{
	size_t i;
	float a;
	return getShortestDistanceAndIndex(ray, a, distanceOut, i);
}*/

bool CurvedMirror::MirrorShape::getShortestDistanceAndIndex(const LightRay& ray, float& distanceA, float& distanceB, size_t& index) const
{
	float factorA, factorB;
	float shortestDistanceA;
	float shortestDistanceB = 99999999;
	long shortestIndex = -1;
	for (size_t i = 0; i < m_mirrorRays.size(); ++i)
	{
		if (m_mirrorRays[i].raycast(ray.ray, factorA, factorB))
		{
			if (factorB < shortestDistanceB)
			{
				if (factorA >= 0 && factorA <= 1 && factorB > 0.1)
				{
					shortestDistanceB = factorB;
					shortestDistanceA = factorA;
					shortestIndex = i;
				}
			}
		}
	}
	if (shortestIndex == -1)
		return false;
	distanceA = shortestDistanceA;
	distanceB = shortestDistanceB;
	index = shortestIndex;
	return true;
}

void CurvedMirror::update()
{
	setRotation(getRotation() + m_rotationSpeed);
}


bool CurvedMirror::MirrorShape::getCollisionData(const LightRay& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{
	float factorA, factorB;

	size_t i;
	if (getShortestDistanceAndIndex(ray, factorA, factorB, i))
	{
		outCollisionFactor = factorB;
		sf::Vector2f collisionPoint = m_mirrorRays[i].getPoint(factorA);
		float subshapeAngle = QSFML::VectorMath::getAngle(m_mirrorRays[i].getDirection());

		outNormalAngle = subshapeAngle + M_PI_2;
		return true;
	}
	return false;
}
