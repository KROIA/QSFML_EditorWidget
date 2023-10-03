#include "OpticalElement.h"


std::vector<OpticalElement*> OpticalElement::s_opticalElements;
OpticalElement::OpticalElement(const std::string& name)
	: CanvasObject(name)
	, m_shape(nullptr)
	, m_n1(1)
	, m_n2(1.5)
	, m_bounceCount(0)
	, m_maxBounceCount(3)
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

bool OpticalElement::processLaser(const QSFML::Utilities::Ray& ray, 
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	if (!m_shape)
		return false;
	sf::Vector2f p1;
	processLaser_intern(ray, reflectedOut, additionalLightPathsOut, p1);
	return true;
}
bool OpticalElement::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& outDistance) const
{
	if (!m_shape)
		return false;
	float normal;
	bool inside;
	return m_shape->getCollisionData(ray, outDistance, normal, inside);
}
void OpticalElement::reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
	ReflectionAndRefractionData &outData)
{
	rayAngle = QSFML::VectorMath::getNormalzedAngle(rayAngle);
	normalAngle = QSFML::VectorMath::getNormalzedAngle(normalAngle);

	// Winkel vom Lichtsrahl gegenüber der Normalen
	float epsilon1 = (normalAngle - rayAngle);
	
	epsilon1 = QSFML::VectorMath::getNormalzedAngle(epsilon1);

	/*if (abs(epsilon1) > M_PI_2)
	{
		normalAngle = QSFML::VectorMath::getNormalzedAngle(M_PI + normalAngle);
		epsilon1 = -(normalAngle - rayAngle);
		epsilon1 = QSFML::VectorMath::getNormalzedAngle(epsilon1);
		//epsilon1 = QSFML::VectorMath::getNormalzedAngle(M_PI_2 - abs(epsilon1));
		//float nextNormal = QSFML::VectorMath::getNormalzedAngle(normalAngle + M_PI);
		//float nextNormal = QSFML::VectorMath::getNormalzedAngle(M_PI + normalAngle);
		// Lichtstrahl trifft von der anderen Seite auf die Grenzschicht. n1 und n2 vertauschen
		//reflectAndRefract(rayAngle, nextNormal, n1, n2, outData);
		//outHasRefraction = false;
		//return;
	}*/

	// Berechnet den Grenzwinkel, bei dessen überschreitung keine Lichtbrechung 
	// mehr passiert sonder totale reflexion auftritt.
	//float boundryAngle = M_PI_2;
	float epsilon2_input = n1 / n2 * sin(epsilon1);
	//if (n1 > n2)
	//	boundryAngle = asin(n2 / n1);

	

	// Berechne reflexion
	outData.reflectAngle = M_PI + epsilon1 + normalAngle;

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
	//if (normalAngle > M_PI_2)
	//	normalAngle = M_PI - normalAngle;
	//outData.refractAngle = normalAngle - epsilon2;
	//return;

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
bool OpticalElement::reflectAndRefract(const QSFML::Utilities::Ray& ray, const Shape& shape, float n1, float n2,
	ReflectionAndRefractionData& outData)
{
	float collisionPointFactor;
	float normalAngle;
	bool rayInsideShape;

	if (!shape.getCollisionData(ray, collisionPointFactor, normalAngle, rayInsideShape))
		return false;

	outData.normalAngle = normalAngle;
	outData.rayCollisionFactor = collisionPointFactor;
	if (rayInsideShape)
		std::swap(n1, n2);
	reflectAndRefract(QSFML::VectorMath::getAngle(ray.getDirection()), normalAngle, n1, n2, outData);
	return true;
}



bool OpticalElement::reflectAndRefract_circleSegment(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
	float circleRadius, float minAngle, float maxAngle, float n1, float n2, bool normalDirToCircleCenter,
	float& outCollisionPointFactor,
	ReflectionAndRefractionData& outData)
{
	float collFac1 = 9999999, collFac2 = 9999999;
	int collisionCount = ray.getCircleCollisionFactors(circleCenter, circleRadius, collFac1, collFac2);
	if (collisionCount == 0)
		return false;

	bool lessZero1 = collFac1 < 0;
	bool lessZero2 = collFac2 < 0;
	if (lessZero1 && lessZero2)
		return false;

	float minFac = collFac1;
	if (lessZero1)
		minFac = collFac2;
	else
	{
		minFac = std::min(collFac1, collFac2);
	}
	outCollisionPointFactor = minFac;
	sf::Vector2f collPoint = ray.getPoint(minFac);

	sf::Vector2f normal = collPoint - circleCenter;
	outData.normalAngle = QSFML::VectorMath::getAngle(normal);
	if (!normalDirToCircleCenter)
		outData.normalAngle = -outData.normalAngle +M_PI;

	if (!QSFML::VectorMath::isAngleInRange(outData.normalAngle, minAngle, maxAngle))
		return false;


	reflectAndRefract(QSFML::VectorMath::getAngle(ray.getDirection()), outData.normalAngle, n1, n2,
		outData);

	return true;
}

bool OpticalElement::reflectAndRefract_circle(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
	float circleRadius, float n1, float n2, bool normalDirToCircleCenter,
	float& outCollisionPointFactor,
	ReflectionAndRefractionData& outData)
{
	float collFac1 = 9999999, collFac2 = 9999999;
	int collisionCount = ray.getCircleCollisionFactors(circleCenter, circleRadius, collFac1, collFac2);
	if (collisionCount == 0)
		return false;

	bool lessZero1 = collFac1 < 0;
	bool lessZero2 = collFac2 < 0;
	if (lessZero1 && lessZero2)
		return false;

	float minFac = collFac1;
	if (lessZero1)
		minFac = collFac2;
	else
	{
		minFac = std::min(collFac1, collFac2);
	}
	outCollisionPointFactor = minFac;
	sf::Vector2f collPoint = ray.getPoint(minFac);

	sf::Vector2f normal = collPoint - circleCenter;
	outData.normalAngle = QSFML::VectorMath::getAngle(normal);
	if (!normalDirToCircleCenter)
		outData.normalAngle = -outData.normalAngle;

	reflectAndRefract(QSFML::VectorMath::getAngle(ray.getDirection()), outData.normalAngle, n1, n2,
		outData);

	return true;
}
bool OpticalElement::reflectAndRefract_line(const QSFML::Utilities::Ray& ray, const sf::Vector2f& pos1, const sf::Vector2f& pos2,
	float& outCollisionPointFactor,
	ReflectionAndRefractionData& outData)
{

	return false;
}



void OpticalElement::processLaser_intern(const QSFML::Utilities::Ray& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut,
	sf::Vector2f& outNextCollisionPoint) const
{
	if (m_bounceCount > m_maxBounceCount)
		return;
	
	ReflectionAndRefractionData data1;
	if (!reflectAndRefract(ray, *m_shape, m_n1, m_n2, data1))
		return;

	++m_bounceCount;
	outNextCollisionPoint = ray.getPoint(data1.rayCollisionFactor);


	QSFML::Utilities::Ray bounced(ray);
	
	sf::Vector2f dirRefract = QSFML::VectorMath::getRotatedUnitVector(data1.refractAngle);
	bounced.setPos(outNextCollisionPoint + 0.001f * dirRefract);
	bounced.setDirection(dirRefract);


	float distance;
	bool doReflectBounce = true;
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

	if(doReflectBounce)
	{
		sf::Vector2f dirReflect = QSFML::VectorMath::getRotatedUnitVector(data1.reflectAngle);
		bounced.setPos(outNextCollisionPoint + 0.001f * dirReflect);
		bounced.setDirection(dirReflect);
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
	--m_bounceCount;

}
/*void OpticalElement::update()
{

}*/