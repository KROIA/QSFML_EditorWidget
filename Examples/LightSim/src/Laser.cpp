#include "Laser.h"
#include "OpticalElement.h"

OBJECT_IMPL(Laser);

Laser::Laser(const std::string& name, QSFML::Objects::GameObjectPtr parent)
	: GameObject(name)
	, m_opticalElements(OpticalElement::getOpticalElements())
{
	m_painter = new LaserPainter(m_lasers);
	m_maxBounces = 100;

	setPosition(sf::Vector2f(0, 0));
	setDirection(sf::Vector2f(1, 0));
	setColor(sf::Color::Red);

	addComponent(m_painter);
}
Laser::Laser(const Laser& other)
	: GameObject(other)
	, m_opticalElements(OpticalElement::getOpticalElements())
{
	m_painter = new LaserPainter(m_lasers);
	m_maxBounces = other.m_maxBounces;
	m_painter->m_color = other.m_painter->m_color;

	addComponent(m_painter);
}
Laser::~Laser()
{

}


void Laser::setPosition(const sf::Vector2f& pos)
{
	m_rootRay.ray.setPosition(pos);
}
const sf::Vector2f& Laser::getPosition() const
{
	return m_rootRay.ray.getPosition();
}
void Laser::setDirection(const sf::Vector2f& dir)
{
	m_rootRay.ray.setDirection(dir);
	m_rootRay.ray.normalize();
}
const sf::Vector2f& Laser::getDirection() const
{
	return m_rootRay.ray.getDirection();
}

void Laser::setColor(const sf::Color& col)
{
	m_painter->m_color = col;
}
const sf::Color& Laser::getColor() const
{
	return m_painter->m_color;
}

void Laser::setMaxBounces(unsigned int max)
{
	m_maxBounces = max;
}
unsigned int Laser::getMaxBounces() const
{
	return m_maxBounces;
}

void Laser::update()
{
	m_lasers.clear();
	m_childRays.clear();
	/*static float rotSpeed = 0.001;
	float range = 5 * M_PI /360;

	float angle = QSFML::VectorMath::getAngleRAD(getDirection());
	if (angle < range - M_PI_2)
	{
		angle += rotSpeed;
	}
	else
	{
		angle = -M_PI_2 - range;
	}
	setDirection(QSFML::VectorMath::getRotatedUnitVectorRAD(angle));*/

	updateRay(m_rootRay, m_rootLaserInfo, 0);
	
}
void Laser::updateRay(LightRay& r, LaserInfo& info, unsigned int iteration)
{
	//OpticalElement* OpticalElement = OpticalElements[i];
	if (iteration > m_maxBounces)
		return;

	LightRay ray = r;

	OpticalElement* nearesElement = nullptr;
	float minDistance = 99999999;

	for (size_t i = 0; i < m_opticalElements.size(); ++i)
	{
		float distance = 99999999;
		if (m_opticalElements[i]->getRaycastDistance(ray, distance))
		{
			if (distance < minDistance)
			{
				nearesElement = m_opticalElements[i];
				minDistance = distance;
			}
		}
	}

	LaserInfo laserInfo(info);
	laserInfo.start = ray.ray.getPosition();
	laserInfo.end = ray.ray.getPoint(minDistance);

	m_lasers.push_back(laserInfo);

	if (nearesElement)
	{
		QSFML::vector<LightRay> reflected;
		QSFML::vector< LaserInfo> additionalLightPaths;
		if (nearesElement->processLaser(ray, reflected, additionalLightPaths))
		{
			if(additionalLightPaths.size())
				m_lasers.insert(m_lasers.end(), additionalLightPaths.begin(), additionalLightPaths.end());
			for (size_t i = 0; i < reflected.size(); ++i)
			{
				updateRay(reflected[i], laserInfo, iteration + 1);
			}
		}
	}
}


Laser::LaserPainter::LaserPainter(QSFML::vector<LaserInfo>& l, const std::string& name)
	: Drawable(name)
	, m_lasers(l)
{

}
Laser::LaserPainter::LaserPainter(const LaserPainter& other)
	: Drawable(other)
	, m_lasers(other.m_lasers)
{

}
COMPONENT_IMPL(Laser::LaserPainter);

void Laser::LaserPainter::drawComponent(sf::RenderTarget& target,
							   sf::RenderStates states) const
{
	for (size_t i = 0; i < m_lasers.size(); ++i)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(m_lasers[i].start, m_color),
			sf::Vertex(m_lasers[i].end, m_color)
		};


		target.draw(line, 2, sf::Lines, states);
	}
}
