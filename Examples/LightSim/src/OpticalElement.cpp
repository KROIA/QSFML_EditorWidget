#include "OpticalElement.h"


std::vector<OpticalElement*> OpticalElement::s_opticalElements;
OpticalElement::OpticalElement(const std::string& name)
	: CanvasObject(name)
{
	s_opticalElements.push_back(this);
}
OpticalElement::OpticalElement(const OpticalElement& other)
	: CanvasObject(other)
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

bool OpticalElement::processLaser(const QSFML::Utilities::Ray& ray, 
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	return false;
}
bool OpticalElement::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
	return false;
}
void OpticalElement::reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
	float& outReflectAngle, float& outRefractAngle, bool& outHasRefraction)
{
	rayAngle = QSFML::VectorMath::getNormalzedAngle(rayAngle);
	normalAngle = QSFML::VectorMath::getNormalzedAngle(normalAngle);

	// Winkel vom Lichtsrahl gegenüber der Normalen
	float epsilon1 = (rayAngle - normalAngle);
	
	epsilon1 = QSFML::VectorMath::getNormalzedAngle(epsilon1);

	if (abs(epsilon1) > M_PI_2)
	{
		//float nextNormal = QSFML::VectorMath::getNormalzedAngle(normalAngle + M_PI);
		float nextNormal = QSFML::VectorMath::getNormalzedAngle(M_PI + normalAngle);
		// Lichtstrahl trifft von der anderen Seite auf die Grenzschicht. n1 und n2 vertauschen
		reflectAndRefract(rayAngle, nextNormal, n1, n2, outReflectAngle, outRefractAngle, outHasRefraction);
		//outHasRefraction = false;
		return;
	}

	// Berechnet den Grenzwinkel, bei dessen überschreitung keine Lichtbrechung 
	// mehr passiert sonder totale reflexion auftritt.
	float boundryAngle = M_PI_2;
	if (n1 > n2)
		boundryAngle = asin(n2 / n1);

	

	// Berechne reflexion
	outReflectAngle = M_PI - (epsilon1 - normalAngle);

	if (abs(epsilon1) > boundryAngle)
	{
		// Keine Brechung --> totale Reflektion
		outHasRefraction = false;
		return;
	}

	outHasRefraction = true;
	// Winkel zwischen der Normalen und dem gebrochenen Strahl
	float epsilon2 = asin(n1 / n2 * sin(epsilon1));
	outRefractAngle =  normalAngle + epsilon2;
}
bool OpticalElement::reflectAndRefract_circleSegment(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
	float circleRadius, float minAngle, float maxAngle, float n1, float n2, bool normalDirToCircleCenter,
	float& outCollisionPointFactor,
	float& outCollisionNormalAngle, float& outReflectAngle, float& outRefractAngle, bool& outHasRefraction)
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
	outCollisionNormalAngle = QSFML::VectorMath::getAngle(normal);
	if (!normalDirToCircleCenter)
		outCollisionNormalAngle = -outCollisionNormalAngle+M_PI;

	if (!QSFML::VectorMath::isAngleInRange(outCollisionNormalAngle, minAngle, maxAngle))
		return false;


	reflectAndRefract(QSFML::VectorMath::getAngle(ray.getDirection()), outCollisionNormalAngle, n1, n2,
		outReflectAngle, outRefractAngle, outHasRefraction);

	return true;
}

bool OpticalElement::reflectAndRefract_circle(const QSFML::Utilities::Ray& ray, const sf::Vector2f& circleCenter,
	float circleRadius, float n1, float n2, bool normalDirToCircleCenter,
	float& outCollisionPointFactor,
	float& outCollisionNormalAngle, float& outReflectAngle, float& outRefractAngle, bool& outHasRefraction)
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
	outCollisionNormalAngle = QSFML::VectorMath::getAngle(normal);
	if (!normalDirToCircleCenter)
		outCollisionNormalAngle = -outCollisionNormalAngle;

	reflectAndRefract(QSFML::VectorMath::getAngle(ray.getDirection()), outCollisionNormalAngle, n1, n2,
		outReflectAngle, outRefractAngle, outHasRefraction);

	return true;
}
/*void OpticalElement::update()
{

}*/