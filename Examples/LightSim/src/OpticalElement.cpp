#include "OpticalElement.h"


std::vector<OpticalElement*> OpticalElement::s_opticalElements;
OpticalElement::OpticalElement(const std::string& name)
	: CanvasObject(name)
	, m_shape(nullptr)
	, m_n1(1)
	, m_n2(1.5)
	, m_bounceCount(0)
	, m_maxBounceCount()
	, m_doesReflect(true)
	, m_doesRefract(true)

{
	s_opticalElements.push_back(this);
}
OpticalElement::OpticalElement(const OpticalElement& other)
	: CanvasObject(other)
	, m_shape(nullptr)
	, m_n1(other.m_n1)
	, m_n2(other.m_n2)
	, m_bounceCount(0)
	, m_maxBounceCount(other.m_bounceCount)
	, m_doesReflect(other.m_doesReflect)
	, m_doesRefract(other.m_doesRefract)
{
	s_opticalElements.push_back(this);
}
OpticalElement::~OpticalElement()
{
	for (size_t i = 0; i < s_opticalElements.size(); ++i)
	{
		if (s_opticalElements[i] == this)
		{
			s_opticalElements.erase(s_opticalElements.begin() + i);
			return;
		}
	}
}
OBJECT_IMPL(OpticalElement);

const std::vector<OpticalElement*> const& OpticalElement::getOpticalElements()
{
	return s_opticalElements;
}

void OpticalElement::setShape(Shape* shape)
{
	m_shape = shape;
}
Shape* OpticalElement::getShape() const
{
	return m_shape;
}

void OpticalElement::setRefractionIndexOutside(float n1)
{
	m_n1 = n1;
}
void OpticalElement::setRefractionIndexInside(float n2)
{
	m_n2 = n2;
}
float OpticalElement::getRefractionIndexInside() const
{
	return m_n1;
}
float OpticalElement::getRefractionIndexOutside() const
{
	return m_n2;
}

void OpticalElement::doesReflect(bool enable)
{
	m_doesReflect = enable;
}

void OpticalElement::doesRefract(bool enable)
{
	m_doesRefract = enable;
}

bool OpticalElement::doesReflect() const
{
	return m_doesReflect;
}

bool OpticalElement::doesRefract() const
{
	return m_doesRefract;
}

bool OpticalElement::processLaser(const LightRay& ray,
	std::vector< LightRay>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	if (!m_shape)
		return false;
	sf::Vector2f p1;
	processLaser_intern(ray, reflectedOut, additionalLightPathsOut, p1);
	return true;
}
bool OpticalElement::getRaycastDistance(const LightRay& ray, float& outDistance) const
{
	if (!m_shape)
		return false;
	float normal;
	bool inside;
	return m_shape->getCollisionData(ray, outDistance, normal, inside);
}
void OpticalElement::reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
	ReflectionAndRefractionData &outData) const
{
	rayAngle = QSFML::VectorMath::getNormalzedAngle(rayAngle);
	normalAngle = QSFML::VectorMath::getNormalzedAngle(normalAngle);

	// Winkel vom Lichtsrahl gegenüber der Normalen
	float epsilon1 = (normalAngle - rayAngle);
	
	epsilon1 = QSFML::VectorMath::getNormalzedAngle(epsilon1);

	// Berechne reflexion
	outData.reflectAngle = M_PI + epsilon1 + normalAngle;


	if (!m_doesRefract)
	{
		outData.doesRefract = false;
		return;
	}

	// Berechnet den Grenzwinkel, bei dessen überschreitung keine Lichtbrechung 
	// mehr passiert sonder totale reflexion auftritt.
	float epsilon2_input = n1 / n2 * sin(epsilon1);

	
	if (abs(epsilon2_input) > 1)
	//if (abs(epsilon1) > boundryAngle)
	{
		// Keine Brechung --> totale Reflektion
		outData.doesRefract = false;
		return;
	}

	outData.doesRefract = true;
	// Winkel zwischen der Normalen und dem gebrochenen Strahl
	float epsilon2 = asin(n1 / n2 * sin(epsilon1));
	if (std::abs(epsilon1) > M_PI_2)
		epsilon2 = M_PI - epsilon2;

	outData.refractAngle = normalAngle - epsilon2;
	return;
	if (std::abs(normalAngle) > M_PI_2)
	{
		outData.refractAngle = (M_PI - normalAngle - epsilon2);
	}
	else
	{
		
		
	}
}

bool OpticalElement::reflectAndRefract(const LightRay& ray, const Shape& shape, float n1, float n2,
	ReflectionAndRefractionData& outData) const
{
	float collisionPointFactor = 0;
	float normalAngle = 0;
	bool rayInsideShape = false;

	if (!shape.getCollisionData(ray, collisionPointFactor, normalAngle, rayInsideShape))
		return false;

	outData.normalAngle = normalAngle;
	outData.rayCollisionFactor = collisionPointFactor;
	if (rayInsideShape)
		std::swap(n1, n2);
	reflectAndRefract(QSFML::VectorMath::getAngle(ray.ray.getDirection()), normalAngle, n1, n2, outData);
	return true;
}

void OpticalElement::processLaser_intern(const LightRay& ray,
	std::vector< LightRay>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut,
	sf::Vector2f& outNextCollisionPoint) const
{
	if (m_bounceCount >= m_maxBounceCount)
		return;
	
	ReflectionAndRefractionData data1;
	if (!reflectAndRefract(ray, *m_shape, m_n1, m_n2, data1))
		return;

	++m_bounceCount;
	outNextCollisionPoint = ray.ray.getPoint(data1.rayCollisionFactor);


	LightRay bounced = ray;
	
	sf::Vector2f dirRefract = QSFML::VectorMath::getRotatedUnitVector(data1.refractAngle);
	bounced.ray.setPos(outNextCollisionPoint + 0.001f * dirRefract);
	bounced.ray.setDirection(dirRefract);


	float distance;
	bool doReflectBounce = true;
	if (m_doesReflect)
	{
		if (data1.doesRefract)
		{
			if (getRaycastDistance(bounced, distance))
			{
				LaserInfo info;
				info.start = outNextCollisionPoint;
				sf::Vector2f point;
				processLaser_intern(bounced, reflectedOut, additionalLightPathsOut, point);
				info.end = point;
				additionalLightPathsOut.push_back(info);
				doReflectBounce = false;
			}
			else
			{
				reflectedOut.push_back(bounced);
				--m_bounceCount;
				return;
			}
		}
	}
	if (m_doesReflect)
	{
		if (doReflectBounce)
		{
			sf::Vector2f dirReflect = QSFML::VectorMath::getRotatedUnitVector(data1.reflectAngle);
			bounced.ray.setPos(outNextCollisionPoint + 0.001f * dirReflect);
			bounced.ray.setDirection(dirReflect);
			if (getRaycastDistance(bounced, distance))
			{
				LaserInfo info;
				info.start = outNextCollisionPoint;
				sf::Vector2f point;
				processLaser_intern(bounced, reflectedOut, additionalLightPathsOut, point);
				info.end = point;
				additionalLightPathsOut.push_back(info);
			}
			else
			{
				reflectedOut.push_back(bounced);
				--m_bounceCount;
				return;
			}
		}
	}
	--m_bounceCount;

}
/*void OpticalElement::update()
{

}*/