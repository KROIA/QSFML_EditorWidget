#include "ConvexLense.h"
#include <qpainter.h>


ConvexLense::ConvexLense(const std::string& name)
	: OpticalElement(name)
{
	m_shape.m_painter = new LenseShape::LensePainter(&m_shape);
	m_shape.m_diameter = 30;
	m_shape.m_angle = M_PI / 4;
	m_shape.m_lenseRadius = 30;
	m_shape.m_pos = sf::Vector2f(0, 0);
	m_n1 = 1;
	m_n2 = 1.5;
	addComponent(m_shape.m_painter);
	m_shape.updateShape();
	setShape(&m_shape);
}
ConvexLense::ConvexLense(const ConvexLense& other)
	: OpticalElement(other)
{
	m_shape.m_painter = new LenseShape::LensePainter(&m_shape);
	m_shape.m_diameter = other.m_shape.m_diameter;
	m_shape.m_angle = other.m_shape.m_angle;
	m_shape.m_lenseRadius = other.m_shape.m_lenseRadius;
	m_shape.m_pos = other.m_shape.m_pos;
	m_n1 = other.m_n1;
	m_n2 = other.m_n2;
	addComponent(m_shape.m_painter);
	m_shape.updateShape();
	setShape(&m_shape);
}
ConvexLense::~ConvexLense()
{

}
OBJECT_IMPL(ConvexLense);


void ConvexLense::setRefractionIndex(float n)
{
	m_n2 = n;
}
float ConvexLense::getRefractionIndex() const
{
	return m_n2;
}
void ConvexLense::setColor(const sf::Color& c)
{
	m_shape.m_painter->m_color = c;
}
const sf::Color& ConvexLense::getColor() const
{
	return m_shape.m_painter->m_color;
}

void ConvexLense::setPos(const sf::Vector2f& pos)
{
	m_shape.m_pos = pos;
	m_shape.updateShape();
}
const sf::Vector2f& ConvexLense::getPos() const
{
	return m_shape.m_pos;
}

void ConvexLense::setRotation(float angle)
{
	m_shape.m_angle = angle;
	m_shape.updateShape();
}
float ConvexLense::getRotation() const
{
	return m_shape.m_angle;
}
void ConvexLense::serDiameter(float w)
{
	m_shape.m_diameter = abs(w);
	m_shape.updateShape();
}
float ConvexLense::getDiameter() const
{
	return m_shape.m_diameter;
}
void ConvexLense::setFocusLength(float r)
{
	m_shape.m_lenseRadius = abs(r);
	m_shape.updateShape();
}
float ConvexLense::getFocusLength() const
{
	return m_shape.m_lenseRadius;
}


/*
bool ConvexLense::processLaser(const QSFML::Utilities::Ray& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	sf::Vector2f collisionPoint;
	return processLaser_intern(ray, reflectedOut, additionalLightPathsOut, false, collisionPoint);
}

bool ConvexLense::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
	float fac = -1;
	float normal; 
	if (getLenseCollisionFactor(ray, false, fac, normal))
	{
		distanceOut = fac;
		return true;
	}
	return false;
}

bool ConvexLense::processLaser_intern(const QSFML::Utilities::Ray& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut, bool outgoingRay,
	sf::Vector2f& collisionPointOut) const
{


	float fac = -1;
	float normalAngle;
	if (getLenseCollisionFactor(ray, outgoingRay, fac, normalAngle))
	{
		collisionPointOut = ray.getPoint(fac);
		float rayAngle = QSFML::VectorMath::getAngle(ray.getDirection());
		

		// {
		// 	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(10, 0.5));
		// 	rect->setFillColor(sf::Color::Blue);
		// 	rect->setPosition(collisionPointOut);
		// 	rect->setRotation(normalAngle * 180 / M_PI);
		// 	m_tmpDraw.push_back(rect);
		// }
		// {
		// 	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(10, 0.5));
		// 	rect->setFillColor(sf::Color::Red);
		// 	rect->setPosition(collisionPointOut);
		// 	rect->setRotation(rayAngle * 180 / M_PI);
		// 	m_tmpDraw.push_back(rect);
		// }

		float n1 = m_n1;
		float n2 = m_n2;
		if (outgoingRay)
		{
			n1 = m_n2;
			n2 = m_n1;
		}

		ReflectionAndRefractionData data;
		reflectAndRefract(rayAngle, normalAngle, n1, n2, data);

		

		if (!outgoingRay && !data.doesRefract)
		{
			QSFML::Utilities::Ray reflected(ray);
			sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVector(data.reflectAngle);
			reflected.setPos(collisionPointOut + 0.001f * dir);
			reflected.setDirection(dir);

			reflectedOut.push_back(reflected);
			LaserInfo info;
			info.start = reflected.getPos();
			info.end = reflected.getPoint(100);
			//additionalLightPathsOut.push_back(info);
		}
		if (data.doesRefract)
		{
			QSFML::Utilities::Ray refracted(ray);
			sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVector(data.refractAngle);
			
			refracted.setDirection(dir);

			if (outgoingRay)
			{
				refracted.setPos(collisionPointOut + 0.001f * dir);
				reflectedOut.push_back(refracted);
			}
			else
			{
				refracted.setPos(collisionPointOut);
				LaserInfo info;
				info.start = refracted.getPos();
				

				refracted.setPos(collisionPointOut + 0.001f * dir);
				sf::Vector2f collisionPoint1;
				processLaser_intern(refracted, reflectedOut, additionalLightPathsOut, true, collisionPoint1);
				if (collisionPoint1.x == 0)
				{
					int a = 0;
				}
				info.end = collisionPoint1;
				additionalLightPathsOut.push_back(info);
			}
		}
		else
		{
			int a = 0;
		}
		return true;
	}
	return false;
}
*/


void ConvexLense::update()
{
	//setRotation(getRotation() + m_rotationSpeed);
}


ConvexLense::LenseShape::LensePainter::LensePainter(LenseShape* lense, const std::string& name)
	: Drawable(name)
	, m_lense(lense)
{
	m_resolution = 20;
}
ConvexLense::LenseShape::LensePainter::LensePainter(const LensePainter& other)
	: Drawable(other)
	, m_lense(nullptr)
{
	m_resolution = other.m_resolution;
}
COMPONENT_IMPL(ConvexLense::LenseShape::LensePainter);


void ConvexLense::LenseShape::LensePainter::draw(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	const size_t resolution = 20;
	sf::Vertex line1[resolution];
	sf::Vertex line2[resolution];

	float angleRangeMin = m_lense->m_angle + m_lense->m_openingAngle / 2;
	float angleRangeMin2 = angleRangeMin + M_PI;
	float angleRangeMax = m_lense->m_angle - m_lense->m_openingAngle / 2;
	float dAngle = (angleRangeMax - angleRangeMin) / (resolution-1);

	float currentAngle1 = angleRangeMin;
	float currentAngle2 = angleRangeMin2;

	float r = m_lense->m_lenseRadius;
	sf::Vector2f pos = m_lense->m_pos;
	sf::Vector2f c1Pos = m_lense->m_circlePos1;
	sf::Vector2f c2Pos = m_lense->m_circlePos2;

	for (size_t i = 0; i < resolution; ++i)
	{
		float deltaAngle = dAngle * i;
		currentAngle1 = angleRangeMin + deltaAngle;
		currentAngle2 = angleRangeMin2 + deltaAngle;

		line1[i].color = m_color;
		line2[i].color = m_color;

		line1[i].position = QSFML::VectorMath::getRotatedUnitVector(currentAngle1) * r + c2Pos;
		line2[i].position = QSFML::VectorMath::getRotatedUnitVector(currentAngle2) * r + c1Pos;

		
	}

	sf::CircleShape circle(2);
	// change the number of sides (points) to 100
	circle.setPointCount(resolution);
	circle.setOrigin(2.f, 2.f);
	circle.setFillColor(m_color);
	circle.setOutlineColor(m_color);
	circle.setPosition(pos);

	target.draw(line1, resolution, sf::LineStrip);
	target.draw(line2, resolution, sf::LinesStrip);
	target.draw(circle);

	{
		sf::CircleShape circle(2);
		// change the number of sides (points) to 100
		circle.setPointCount(resolution);
		circle.setOrigin(2.f, 2.f);
		circle.setFillColor(m_color);
		circle.setOutlineColor(m_color);
		circle.setPosition(m_lense->m_circlePos1);
		target.draw(circle);
		circle.setPosition(m_lense->m_circlePos2);
		target.draw(circle);

	}


	for (size_t i = 0; i < m_lense->m_tmpDraw.size(); ++i)
	{
		target.draw(*m_lense->m_tmpDraw[i]);
		delete m_lense->m_tmpDraw[i];
	}
	m_lense->m_tmpDraw.clear();
}


void ConvexLense::LenseShape::updateShape()
{
	m_openingAngle = asin(m_diameter / (2 * m_lenseRadius)) * 2;
	float centerOffset = sqrt(m_lenseRadius * m_lenseRadius - m_diameter * m_diameter / 4);
	m_circlePos1 = QSFML::VectorMath::getRotatedUnitVector(m_angle) * centerOffset;
	m_circlePos2 = m_pos - m_circlePos1;
	m_circlePos1 += m_pos;

	/*sf::Vector2f direction = QSFML::VectorMath::getRotatedUnitVector(m_angle)
	sf::Vector2f start = m_pos - direction * m_width / 2.f;
	sf::Vector2f end = m_pos + direction * m_width / 2.f;

	m_ConvexLenseLine->setStartPos(start);
	m_ConvexLenseLine->setEndPos(end);

	m_ConvexLenseRay.setPos(start);
	m_ConvexLenseRay.setDirection(direction * m_width);*/
}


bool ConvexLense::LenseShape::getLenseCollisionFactor(const QSFML::Utilities::Ray& ray, bool outgoingRay, float& smalestFactor, float& collisionNormal, bool& rayInsideShape) const
{
	float fac1 = 99999999;
	float fac2 = 99999999;

	float angleRangeMin = M_PI + m_angle - m_openingAngle / 2;
	float angleRangeMax = M_PI + m_angle + m_openingAngle / 2;

	bool isInside1 = false;
	bool isInside2 = false;


	/*if (outgoingRay)
	{
		angleRangeMin += M_PI;
		angleRangeMax += M_PI;
	}*/

	bool c1 = getCircleElementCollisionFactor(ray, m_circlePos1, m_lenseRadius, angleRangeMin, angleRangeMax, fac1, isInside1);
	/*if (outgoingRay)
	{
		angleRangeMin -= M_PI;
		angleRangeMax -= M_PI;
	}
	else
	{
		angleRangeMin += M_PI;
		angleRangeMax += M_PI;
	}*/
	angleRangeMin -= M_PI;
	angleRangeMax -= M_PI;
	bool c2 = getCircleElementCollisionFactor(ray, m_circlePos2, m_lenseRadius, angleRangeMin, angleRangeMax, fac2, isInside2);

	rayInsideShape = isInside1 && isInside2;

	if (!(c1 + c2))
		return false;

	if (fac1 < fac2 && fac1 > 0)
	{
		smalestFactor = fac1;
		sf::Vector2f normal = m_circlePos1 - ray.getPoint(smalestFactor);
		collisionNormal = QSFML::VectorMath::getAngle(normal);
	}
	else if (fac2 > 0)
	{
		smalestFactor = fac2;
		sf::Vector2f normal = m_circlePos2 - ray.getPoint(smalestFactor);
		collisionNormal = QSFML::VectorMath::getAngle(normal);
	}
	else
		return false;
	return true;
}

// calculates the malest factor in ray which collides on the edge of one part of the lense
bool ConvexLense::LenseShape::getCircleElementCollisionFactor(const QSFML::Utilities::Ray& ray, const sf::Vector2f& center,
	float radius, float minAngle, float maxAngle, float& smalestFactor, bool& rayInsideSegment) const
{

	float fac1 = -1;
	float fac2 = -1;
	if (!getCircleCollisionFactor(ray, center, radius, fac1, fac2))
		return false;


	float angle1 = 0;
	float angle2 = 0;

	float minFac;
	float maxFac;
	if (fac1 > fac2)
	{
		minFac = fac2;
		maxFac = fac1;
	}
	else
	{
		minFac = fac1;
		maxFac = fac2;
	}


	
	if (minFac > 0)
	{
		angle1 = QSFML::VectorMath::getAngle(ray.getPoint(minFac) - center);
		float posAngle = QSFML::VectorMath::getAngle(center - ray.getPos());
		rayInsideSegment = (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle) ||
			QSFML::VectorMath::isAngleInRange(M_PI - angle2, minAngle, maxAngle)) &&
			QSFML::VectorMath::getLength(ray.getPos() - center) < radius;
		if (QSFML::VectorMath::isAngleInRange(angle1, minAngle, maxAngle))
		{
			smalestFactor = minFac;
			return true;
		}
	}

	
	if (maxFac > 0)
	{
		angle2 = QSFML::VectorMath::getAngle(ray.getPoint(maxFac) - center);
		float posAngle = QSFML::VectorMath::getAngle(center - ray.getPos());
		
		rayInsideSegment = (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle)||
			QSFML::VectorMath::isAngleInRange(M_PI - angle2, minAngle, maxAngle))&&
			QSFML::VectorMath::getLength(ray.getPos() - center) < radius;
		if (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle))
		{
			smalestFactor = maxFac;
			return true;
		}
	}
	return false;
}

// calculates the factor in ray which collides on the edge of the circle
bool ConvexLense::LenseShape::getCircleCollisionFactor(const QSFML::Utilities::Ray& ray, const sf::Vector2f& center,
	float radius, float& fac1, float& fac2) const
{
	bool failed;
	float shortestFactorToCenter = ray.getShortestDistanceFactor(center, &failed);
	if (failed)
		return false; // No collision on circle

	/*sf::Vector2f shortestPointToCenter = ray.getPoint(shortestFactorToCenter);

	sf::CircleShape* circle = new sf::CircleShape(1);
	// change the number of sides (points) to 100
	circle->setPointCount(20);
	circle->setOrigin(1.f, 1.f);
	circle->setFillColor(sf::Color::Yellow);
	circle->setOutlineColor(sf::Color::Yellow);
	circle->setPosition(shortestPointToCenter);
	m_tmpDraw.push_back(circle);



	float rayDistanceToCenter = QSFML::VectorMath::getLength(center - shortestPointToCenter);
	float angle = QSFML::VectorMath::getAngle(center - shortestPointToCenter);
	sf::RectangleShape *line = new sf::RectangleShape(sf::Vector2f(rayDistanceToCenter, 0.2));
	line->setPosition(shortestPointToCenter);
	line->rotate(angle*180/M_PI);
	m_tmpDraw.push_back(line);*/


	sf::Vector2f bd = ray.getDirection();
	sf::Vector2f bp = ray.getPos();

	float bdx2 = bd.x * bd.x;
	float bdy2 = bd.y * bd.y;

	float bpy2 = bp.y * bp.y;
	float bpx2 = bp.x * bp.x;

	float xp2 = center.x * center.x;
	float yp2 = center.y * center.y;
	float r2 = radius * radius;

	float bpx_xp = bp.x - center.x;
	float bpy_yp = bp.y - center.y;

	float diskrim = (bdx2 + bdy2) * r2 - bdx2 * (bpy2 - 2 * bp.y * center.y + yp2) + 2 * bd.x * bd.y * (bpx_xp) * (bpy_yp)-bdy2 * (bpx2 - 2 * bp.x * center.x + xp2);
	float divisor = (bdx2 + bdy2);
	float tmp1 = bd.x * (bpx_xp)+bd.y * (bpy_yp);
	float root = sqrt(diskrim);
	float t1 = ((root - tmp1) / divisor);
	float t2 = -((root + tmp1) / divisor);

	if (t1 < 0 && t2 < 0)
		return false;

	fac1 = t1;
	fac2 = t2;
	return true;
}





bool ConvexLense::LenseShape::getRaycastData(const QSFML::Utilities::Ray& ray,
	const sf::Vector2f& circleOrigin,
	float& alpha1, float& alpha2,
	float& distance1, float& distance2,
	sf::Vector2f& point1,
	sf::Vector2f& point2) const
{
	bool failed;
	float shortestDistanceFactor = ray.getShortestDistanceFactor(circleOrigin, &failed);
	if (failed)
		return false;

	sf::Vector2f shortestDistancePoint = ray.getPoint(shortestDistanceFactor);
	float gamma = QSFML::VectorMath::getAngle(ray.getDirection());
	float minDistanceToCenter = QSFML::VectorMath::getLength(shortestDistancePoint - circleOrigin);
	float beta = acos(minDistanceToCenter / m_lenseRadius);
	float gamma2 = gamma - M_PI_2;
	alpha1 = gamma2 - beta;
	alpha2 = gamma2 + beta;

	point1.x = cos(alpha1) * m_lenseRadius;
	point1.y = sin(alpha1) * m_lenseRadius;

	point2.x = cos(alpha2) * m_lenseRadius;
	point2.y = sin(alpha2) * m_lenseRadius;

	distance1 = QSFML::VectorMath::getLength(ray.getPos() - point1);
	distance2 = QSFML::VectorMath::getLength(ray.getPos() - point2);


	return true;
}
/*void ConvexLense::LenseShape::getCollisionData(float alpha, float rayAngle, float& entryAngle, sf::Vector2f& point)
{

}*/

bool ConvexLense::LenseShape::getCollisionData(const QSFML::Utilities::Ray& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{


	float fac = -1;
	float normalAngle;
	bool isInside;
	if (getLenseCollisionFactor(ray, false, fac, normalAngle, isInside))
	{
		outCollisionFactor = fac;
		outNormalAngle = normalAngle;
		rayStartsInsideShape = isInside;
		return true;
	}
	return false;
}