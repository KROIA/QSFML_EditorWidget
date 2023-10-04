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
	setThickness(20);
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
void ConvexLense::setThickness(float t)
{
	m_shape.setThickness(t);
}
float ConvexLense::getThickness() const
{
	return m_shape.getThickness();
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
	
	std::vector < sf::Vertex> left;
	std::vector < sf::Vertex> right;
	m_lense->getPainterVertecies(left, right, resolution);

	for (size_t i = 0; i < resolution; ++i)
	{
		left[i].color = m_color;
		right[i].color = m_color;
	}

	target.draw(left.data(), resolution, sf::LineStrip);
	target.draw(right.data(), resolution, sf::LineStrip);


	/*
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

	}*/


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

	float d2 = m_diameter / 2;
	m_parableParamB = -m_lenseThickness/2;

	m_parableParamA = m_lenseThickness / (2 * d2 * d2);


	/*sf::Vector2f direction = QSFML::VectorMath::getRotatedUnitVector(m_angle)
	sf::Vector2f start = m_pos - direction * m_width / 2.f;
	sf::Vector2f end = m_pos + direction * m_width / 2.f;

	m_ConvexLenseLine->setStartPos(start);
	m_ConvexLenseLine->setEndPos(end);

	m_ConvexLenseRay.setPos(start);
	m_ConvexLenseRay.setDirection(direction * m_width);*/
}

void ConvexLense::LenseShape::setThickness(float t)
{
	m_lenseThickness = t;
	

	
	updateShape();
}
float ConvexLense::LenseShape::getThickness() const
{
	return m_lenseThickness;
}

bool ConvexLense::LenseShape::getLenseCollisionFactor(const LightRay& ray, bool outgoingRay, float& smalestFactor, float& collisionNormal, bool& rayInsideShape) const
{
	float fac1 = 99999999;
	float fac2 = 99999999;

	float angleRangeMin = M_PI + m_angle - m_openingAngle / 2;
	float angleRangeMax = M_PI + m_angle + m_openingAngle / 2;

	bool isInside1 = false;
	bool isInside2 = false;

	bool c1 = getCircleElementCollisionFactor(ray, m_circlePos1, m_lenseRadius, angleRangeMin, angleRangeMax, fac1, isInside1);

	angleRangeMin -= M_PI;
	angleRangeMax -= M_PI;
	bool c2 = getCircleElementCollisionFactor(ray, m_circlePos2, m_lenseRadius, angleRangeMin, angleRangeMax, fac2, isInside2);

	rayInsideShape = isInside1 && isInside2;

	if (!(c1 + c2))
		return false;

	if (fac1 < fac2 && fac1 > 0)
	{
		smalestFactor = fac1;
		sf::Vector2f normal = m_circlePos1 - ray.ray.getPoint(smalestFactor);
		collisionNormal = QSFML::VectorMath::getAngle(normal);
	}
	else if (fac2 > 0)
	{
		smalestFactor = fac2;
		sf::Vector2f normal = m_circlePos2 - ray.ray.getPoint(smalestFactor);
		collisionNormal = QSFML::VectorMath::getAngle(normal);
	}
	else
		return false;
	return true;
}

// calculates the malest factor in ray which collides on the edge of one part of the lense
bool ConvexLense::LenseShape::getCircleElementCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
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
		angle1 = QSFML::VectorMath::getAngle(ray.ray.getPoint(minFac) - center);
		float posAngle = QSFML::VectorMath::getAngle(center - ray.ray.getPos());
		rayInsideSegment = (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle) ||
			QSFML::VectorMath::isAngleInRange(M_PI - angle2, minAngle, maxAngle)) &&
			QSFML::VectorMath::getLength(ray.ray.getPos() - center) < radius;
		if (QSFML::VectorMath::isAngleInRange(angle1, minAngle, maxAngle))
		{
			smalestFactor = minFac;
			return true;
		}
	}

	
	if (maxFac > 0)
	{
		angle2 = QSFML::VectorMath::getAngle(ray.ray.getPoint(maxFac) - center);
		float posAngle = QSFML::VectorMath::getAngle(center - ray.ray.getPos());
		
		rayInsideSegment = (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle)||
			QSFML::VectorMath::isAngleInRange(M_PI - angle2, minAngle, maxAngle))&&
			QSFML::VectorMath::getLength(ray.ray.getPos() - center) < radius;
		if (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle))
		{
			smalestFactor = maxFac;
			return true;
		}
	}
	return false;
}

// calculates the factor in ray which collides on the edge of the circle
bool ConvexLense::LenseShape::getCircleCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
	float radius, float& fac1, float& fac2) const
{
	bool failed;
	float shortestFactorToCenter = ray.ray.getShortestDistanceFactor(center, &failed);
	if (failed)
		return false; // No collision on circle

	sf::Vector2f bd = ray.ray.getDirection();
	sf::Vector2f bp = ray.ray.getPos();

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





bool ConvexLense::LenseShape::getRaycastData(const LightRay& ray,
	const sf::Vector2f& circleOrigin,
	float& alpha1, float& alpha2,
	float& distance1, float& distance2,
	sf::Vector2f& point1,
	sf::Vector2f& point2) const
{
	bool failed;
	float shortestDistanceFactor = ray.ray.getShortestDistanceFactor(circleOrigin, &failed);
	if (failed)
		return false;

	sf::Vector2f shortestDistancePoint = ray.ray.getPoint(shortestDistanceFactor);
	float gamma = QSFML::VectorMath::getAngle(ray.ray.getDirection());
	float minDistanceToCenter = QSFML::VectorMath::getLength(shortestDistancePoint - circleOrigin);
	float beta = acos(minDistanceToCenter / m_lenseRadius);
	float gamma2 = gamma - M_PI_2;
	alpha1 = gamma2 - beta;
	alpha2 = gamma2 + beta;

	point1.x = cos(alpha1) * m_lenseRadius;
	point1.y = sin(alpha1) * m_lenseRadius;

	point2.x = cos(alpha2) * m_lenseRadius;
	point2.y = sin(alpha2) * m_lenseRadius;

	distance1 = QSFML::VectorMath::getLength(ray.ray.getPos() - point1);
	distance2 = QSFML::VectorMath::getLength(ray.ray.getPos() - point2);


	return true;
}
/*void ConvexLense::LenseShape::getCollisionData(float alpha, float rayAngle, float& entryAngle, sf::Vector2f& point)
{

}*/


bool ConvexLense::LenseShape::getParableData(const LightRay& ray, float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{
	float nan = 999999;
	float parable1Fac1 = nan;
	float parable1Fac2 = nan;
	bool parable1Fac1Valid = false;
	bool parable1Fac2Valid = false;
	float parabel1Normal1 = nan;
	float parabel1Normal2 = nan;

	float parable1Rotation = m_angle - M_PI_2;
	sf::Vector2f originDir = QSFML::VectorMath::getRotatedUnitVector(m_angle) * m_parableParamB;
	sf::Vector2f parable1Pos = m_pos - originDir;

	float parable2Fac1 = nan;
	float parable2Fac2 = nan;
	bool parable2Fac1Valid = false;
	bool parable2Fac2Valid = false;
	float parabel2Normal1 = nan;
	float parabel2Normal2 = nan;

	float parable2Rotation = m_angle + M_PI_2;
	sf::Vector2f parable2Pos = m_pos + originDir;

	bool c1 = getParableCollisionFactor(ray.ray, m_parableParamA, m_parableParamB, parable1Rotation, parable1Pos, m_diameter / 2,
										parable1Fac1, parable1Fac2, 
										parabel1Normal1, parabel1Normal2,
										parable1Fac1Valid, parable1Fac2Valid);
	bool c2 = false;
	//bool c2 = getParableCollisionFactor(ray.ray, m_parableParamA, m_parableParamB, parable2Rotation, parable2Pos, m_diameter / 2,
	//									parable2Fac1, parable2Fac2, 
	//									parabel2Normal1, parabel2Normal2,
	//								    parable2Fac1Valid, parable2Fac2Valid);
										
	if (!(c1 + c2))
		return false;

	
	if (!parable1Fac1Valid || !c1 || parable1Fac1 < 0) parable1Fac1 = nan;
	if (!parable1Fac2Valid || !c1 || parable1Fac2 < 0) parable1Fac2 = nan;
	if (!parable2Fac1Valid || !c2 || parable2Fac1 < 0) parable2Fac1 = nan;
	if (!parable2Fac2Valid || !c2 || parable2Fac2 < 0) parable2Fac2 = nan;

	float minFac = parable1Fac1;
	float normal = parabel1Normal1;

	if (parable1Fac2 < minFac)
	{
		minFac = parable1Fac2;
		normal = parabel1Normal2;
	}else if (parable2Fac1 < minFac)
	{
		minFac = parable2Fac1;
		normal = parabel2Normal1;
	}else if (parable2Fac2 < minFac)
	{
		minFac = parable2Fac2;
		normal = parabel2Normal2;
	}
	if (minFac >= nan)
		return false;

	outCollisionFactor = minFac;
	outNormalAngle = normal;
	rayStartsInsideShape = false;

	return true;
}

bool ConvexLense::LenseShape::getParableCollisionFactor(const QSFML::Utilities::Ray& ray, float a, float b, float parableRotation,
	const sf::Vector2f& parablePos, float minMaxInputRange, 
	float& outFac1, float& outFac2, 
	float& outNormal1, float& outNormal2,
	bool& fac1InRange, bool& fac2InRange) const
{
	sf::Vector2f rayPos = QSFML::VectorMath::getRotated(ray.getPos(), -parableRotation);
	sf::Vector2f rayDir = QSFML::VectorMath::getRotated(ray.getDirection(), -parableRotation);

	float bpx = rayPos.x;
	float bpy = rayPos.y;

	float bdx = rayDir.x;
	float bdy = rayDir.y;

	float bdx2 = bdx * bdx;
	float bdy2 = bdy * bdy;

	float diskrim = bdy2 - 4 * a * b * bdx2 + 4 * a * bdx2 * bpy - 4 * a * bdx * bdy * bpx;

	if (diskrim < 0)
	{
		// No collision
		return false;
	}
	float divisor = 2 * a * bdx2;
	if (divisor == 0)
	{
		// No collision
		return false;
	}

	divisor = 1 / divisor;

	float sqrRoot = sqrt(diskrim);
	float adder = bdy - 2 * a * bdx * bpx;


	/*
	( (bdy ^ 2 - 4 * a * b * bdx ^ 2 + 4 * a * bdx ^ 2 * bpy - 4 * a * bdx * bdy * bpx) ^ (1 / 2) + bdy - 2 * a * bdx * bpx)  / (2 * a * bdx ^ 2);
	-((bdy ^ 2 - 4 * a * b * bdx ^ 2 + 4 * a * bdx ^ 2 * bpy - 4 * a * bdx * bdy * bpx) ^ (1 / 2) - bdy + 2 * a * bdx * bpx) / (2 * a * bdx ^ 2)


	t =  ((bdy^2 - 4*a*b*bdx^2 + 4*a*bdx^2*bpy - 4*a*bdx*bdy*bpx)^(1/2) + bdy - 2*a*bdx*bpx)/(2*a*bdx^2);
		(-(bdy^2 - 4*a*b*bdx^2 + 4*a*bdx^2*bpy - 4*a*bdx*bdy*bpx)^(1/2) + bdy - 2*a*bdx*bpx)/(2*a*bdx^2)
	*/
	


	outFac1 = ( sqrRoot + adder) * divisor;
	outFac2 = (-sqrRoot + adder) * divisor;
	
	sf::CircleShape* c1 = new sf::CircleShape(1, 10);
	c1->setOrigin(1, 1);
	c1->setPosition(ray.getPoint(outFac1));
	m_tmpDraw.push_back(c1);
	sf::CircleShape* c2 = new sf::CircleShape(*c1);
	c2->setPosition(ray.getPoint(outFac2));
	m_tmpDraw.push_back(c2);

	float x1 = bpx + outFac1 * bdx;
	float x2 = bpx + outFac2 * bdx;

	outNormal1 = QSFML::VectorMath::getAngle(sf::Vector2f(x1, 2 * a * x1)) - M_PI_2 + parableRotation;
	outNormal2 = QSFML::VectorMath::getAngle(sf::Vector2f(x2, 2 * a * x2)) - M_PI_2 + parableRotation;

	fac1InRange = true;
	fac2InRange = true;
	if (std::abs(x1) > minMaxInputRange)
	{
		fac1InRange = false;
	}
	if (std::abs(x2) > minMaxInputRange)
	{
		fac2InRange = false;
	}

	return true;


}

void ConvexLense::LenseShape::getPainterVertecies(std::vector < sf::Vertex>& left, std::vector < sf::Vertex>& right, size_t resolution)
{
	float dx = m_diameter / (resolution-1);

	float start = -m_diameter / 2;
	float x;
	for (size_t i = 0; i < resolution; ++i)
	{
		x = start + i * dx;
		float y = m_parableParamA * x * x;
		sf::Vector2f pLeft = QSFML::VectorMath::getRotated(sf::Vector2f(x, y + m_parableParamB), m_angle + M_PI_2) + m_pos;
		sf::Vector2f pRight = QSFML::VectorMath::getRotated(sf::Vector2f(x, y + m_parableParamB), m_angle - M_PI_2) + m_pos;

		left.push_back(pLeft);
		right.push_back(pRight);
	}
	
}



bool ConvexLense::LenseShape::getCollisionData(const LightRay& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{

	return getParableData(ray, outCollisionFactor, outNormalAngle, rayStartsInsideShape);
	
	/*float fac = -1;
	float normalAngle;
	bool isInside;
	if (getLenseCollisionFactor(ray, false, fac, normalAngle, isInside))
	{
		outCollisionFactor = fac;
		outNormalAngle = normalAngle;
		rayStartsInsideShape = isInside;
		return true;
	}
	return false;*/
}