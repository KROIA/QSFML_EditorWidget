#include "ConvexLens.h"
#include <qpainter.h>

//#define USE_CIRCLE
#define USE_PARABLE
//#define USE_HYPERBOL

ConvexLens::ConvexLens(const std::string& name)
	: OpticalElement(name)
{
	m_shape.m_painter = new LensShape::LensPainter(&m_shape);
	m_shape.m_diameter = 30;
	m_shape.m_angle = M_PI / 4;
	m_shape.m_lensRadius = 30;
	m_shape.m_pos = sf::Vector2f(0, 0);
	m_shape.m_lens = this;
	setThickness(20);
	addComponent(m_shape.m_painter);
	m_shape.updateShape();
	setShape(&m_shape);
}
ConvexLens::ConvexLens(const ConvexLens& other)
	: OpticalElement(other)
{
	m_shape.m_painter = new LensShape::LensPainter(&m_shape);
	m_shape.m_diameter = other.m_shape.m_diameter;
	m_shape.m_angle = other.m_shape.m_angle;
	m_shape.m_lensRadius = other.m_shape.m_lensRadius;
	m_shape.m_pos = other.m_shape.m_pos;
	m_shape.m_lens = this;
	addComponent(m_shape.m_painter);
	m_shape.updateShape();
	setShape(&m_shape);
}
ConvexLens::~ConvexLens()
{

}
OBJECT_IMPL(ConvexLens);


void ConvexLens::setColor(const sf::Color& c)
{
	m_shape.m_painter->m_color = c;
}
const sf::Color& ConvexLens::getColor() const
{
	return m_shape.m_painter->m_color;
}

void ConvexLens::setPos(const sf::Vector2f& pos)
{
	m_shape.m_pos = pos;
	m_shape.updateShape();
}
const sf::Vector2f& ConvexLens::getPos() const
{
	return m_shape.m_pos;
}

void ConvexLens::setRotation(float angle)
{
	m_shape.m_angle = angle;
	m_shape.updateShape();
}
float ConvexLens::getRotation() const
{
	return m_shape.m_angle;
}
void ConvexLens::setDiameter(float w)
{
	m_shape.m_diameter = abs(w);
	m_shape.updateShape();
}
float ConvexLens::getDiameter() const
{
	return m_shape.m_diameter;
}
void ConvexLens::setThickness(float t)
{
	m_shape.setThickness(t);
}
float ConvexLens::getThickness() const
{
	return m_shape.getThickness();
}
void ConvexLens::setFocusLength(float r)
{
	m_shape.m_lensRadius = abs(r);
	m_shape.updateShape();
}
float ConvexLens::getFocusLength() const
{
	return m_shape.getFocusLength();
	//return m_shape.m_lensRadius;
}


/*
bool ConvexLens::processLaser(const QSFML::Utilities::Ray& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut) const
{
	sf::Vector2f collisionPoint;
	return processLaser_intern(ray, reflectedOut, additionalLightPathsOut, false, collisionPoint);
}

bool ConvexLens::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
{
	float fac = -1;
	float normal; 
	if (getLensCollisionFactor(ray, false, fac, normal))
	{
		distanceOut = fac;
		return true;
	}
	return false;
}

bool ConvexLens::processLaser_intern(const QSFML::Utilities::Ray& ray,
	std::vector< QSFML::Utilities::Ray>& reflectedOut,
	std::vector< LaserInfo>& additionalLightPathsOut, bool outgoingRay,
	sf::Vector2f& collisionPointOut) const
{


	float fac = -1;
	float normalAngle;
	if (getLensCollisionFactor(ray, outgoingRay, fac, normalAngle))
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


void ConvexLens::update()
{
	//setRotation(getRotation() + 0.01);
}


ConvexLens::LensShape::LensPainter::LensPainter(LensShape* lense, const std::string& name)
	: Drawable(name)
	, m_lens(lense)
{
	m_resolution = 20;
}
ConvexLens::LensShape::LensPainter::LensPainter(const LensPainter& other)
	: Drawable(other)
	, m_lens(nullptr)
{
	m_resolution = other.m_resolution;
}
COMPONENT_IMPL(ConvexLens::LensShape::LensPainter);


void ConvexLens::LensShape::LensPainter::draw(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	const size_t resolution = 20;
#ifdef USE_PARABLE
	
	
	std::vector < sf::Vertex> left;
	std::vector < sf::Vertex> right;
	m_lens->getPainterVertecies(left, right, resolution);

	for (size_t i = 0; i < resolution; ++i)
	{
		left[i].color = m_color;
		right[i].color = m_color;
	}

	target.draw(left.data(), resolution, sf::LineStrip);
	target.draw(right.data(), resolution, sf::LineStrip);

#endif

#ifdef USE_CIRCLE
	
	sf::Vertex line1[resolution];
	sf::Vertex line2[resolution];
	float angleRangeMin = m_lens->m_angle + m_lens->m_openingAngle / 2;
	float angleRangeMin2 = angleRangeMin + M_PI;
	float angleRangeMax = m_lens->m_angle - m_lens->m_openingAngle / 2;
	float dAngle = (angleRangeMax - angleRangeMin) / (resolution-1);

	float currentAngle1 = angleRangeMin;
	float currentAngle2 = angleRangeMin2;

	float r = m_lens->m_lensRadius;
	sf::Vector2f pos = m_lens->m_pos;
	sf::Vector2f c1Pos = m_lens->m_circlePos1;
	sf::Vector2f c2Pos = m_lens->m_circlePos2;

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

	
	float brennweite = m_lens->getFocusLength();

	sf::Vector2f brennDir = QSFML::VectorMath::getRotatedUnitVector(m_lens->m_angle) * brennweite;
	circle.setFillColor(sf::Color::Red);
	circle.setPosition(pos + brennDir);
	target.draw(circle);
	circle.setPosition(pos - brennDir);
	target.draw(circle);



	{
		sf::CircleShape circle(1);
		// change the number of sides (points) to 100
		circle.setPointCount(resolution);
		circle.setOrigin(1.f, 1.f);
		circle.setFillColor(m_color);
		circle.setOutlineColor(m_color);
		circle.setPosition(m_lens->m_circlePos1);
		target.draw(circle);
		circle.setPosition(m_lens->m_circlePos2);
		target.draw(circle);

	}
#endif

	for (size_t i = 0; i < m_lens->m_tmpDraw.size(); ++i)
	{
		target.draw(*m_lens->m_tmpDraw[i]);
		delete m_lens->m_tmpDraw[i];
	}
	m_lens->m_tmpDraw.clear();
}


void ConvexLens::LensShape::updateShape()
{
	m_openingAngle = asin(m_diameter / (2 * m_lensRadius)) * 2;
	float centerOffset = sqrt(m_lensRadius * m_lensRadius - m_diameter * m_diameter / 4);
	m_circlePos1 = QSFML::VectorMath::getRotatedUnitVector(m_angle) * centerOffset;
	m_circlePos2 = m_pos - m_circlePos1;
	m_circlePos1 += m_pos;

	float d2 = m_diameter / 2;
	m_parableParamB = -m_lensThickness/2;

	m_parableParamA = m_lensThickness / (2 * d2 * d2);


	/*sf::Vector2f direction = QSFML::VectorMath::getRotatedUnitVector(m_angle);
	sf::Vector2f start = m_pos - direction * m_width / 2.f;
	sf::Vector2f end = m_pos + direction * m_width / 2.f;

	m_ConvexLensLine->setStartPos(start);
	m_ConvexLensLine->setEndPos(end);

	m_ConvexLensRay.setPos(start);
	m_ConvexLensRay.setDirection(direction * m_width);*/
}

void ConvexLens::LensShape::setThickness(float t)
{
	m_lensThickness = t;
	

	
	updateShape();
}
float ConvexLens::LensShape::getThickness() const
{
	return m_lensThickness;
}
float  ConvexLens::LensShape::getFocusLength() const
{
	float rInf = m_lensRadius / 2;
	float n_1 = 1/( m_lens->getRefractionIndexInside() - m_lens->getRefractionIndexOutside());

	return ((n_1) * (rInf));
}

bool ConvexLens::LensShape::getLensCollisionFactor(const LightRay& ray, bool outgoingRay, float& smalestFactor, float& collisionNormal, bool& rayInsideShape) const
{
	float fac1 = 99999999;
	float fac2 = 99999999;

	float angleRangeMin = M_PI + m_angle - m_openingAngle / 2;
	float angleRangeMax = M_PI + m_angle + m_openingAngle / 2;

	bool isInside1 = false;
	bool isInside2 = false;

	bool c1 = getCircleElementCollisionFactor(ray, m_circlePos1, m_lensRadius, angleRangeMin, angleRangeMax, fac1, isInside1);

	angleRangeMin -= M_PI;
	angleRangeMax -= M_PI;
	bool c2 = getCircleElementCollisionFactor(ray, m_circlePos2, m_lensRadius, angleRangeMin, angleRangeMax, fac2, isInside2);

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
bool ConvexLens::LensShape::getCircleElementCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
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


	rayInsideSegment = QSFML::VectorMath::getLength(ray.ray.getPos() - center) < radius;
	if (minFac > 0)
	{
		angle1 = QSFML::VectorMath::getAngle(ray.ray.getPoint(minFac) - center);

		if (QSFML::VectorMath::isAngleInRange(angle1, minAngle, maxAngle))
		{
			smalestFactor = minFac;
			return true;
		}
	}

	
	if (maxFac > 0)
	{
		angle2 = QSFML::VectorMath::getAngle(ray.ray.getPoint(maxFac) - center);

		if (QSFML::VectorMath::isAngleInRange(angle2, minAngle, maxAngle))
		{
			smalestFactor = maxFac;
			return true;
		}
	}
	return false;
}

// calculates the factor in ray which collides on the edge of the circle
bool ConvexLens::LensShape::getCircleCollisionFactor(const LightRay& ray, const sf::Vector2f& center,
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





bool ConvexLens::LensShape::getRaycastData(const LightRay& ray,
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
	float beta = acos(minDistanceToCenter / m_lensRadius);
	float gamma2 = gamma - M_PI_2;
	alpha1 = gamma2 - beta;
	alpha2 = gamma2 + beta;

	point1.x = cos(alpha1) * m_lensRadius;
	point1.y = sin(alpha1) * m_lensRadius;

	point2.x = cos(alpha2) * m_lensRadius;
	point2.y = sin(alpha2) * m_lensRadius;

	distance1 = QSFML::VectorMath::getLength(ray.ray.getPos() - point1);
	distance2 = QSFML::VectorMath::getLength(ray.ray.getPos() - point2);


	return true;
}
/*void ConvexLens::LensShape::getCollisionData(float alpha, float rayAngle, float& entryAngle, sf::Vector2f& point)
{

}*/


bool ConvexLens::LensShape::getParableData(const LightRay& ray, float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{
	double nan = 999999;
	double parable1Fac1 = nan;
	double parable1Fac2 = nan;
	bool parable1Fac1Valid = false;
	bool parable1Fac2Valid = false;
	double parabel1Normal1 = nan;
	double parabel1Normal2 = nan;

	double parable1Rotation = m_angle - M_PI_2;
	sf::Vector2f originDir = QSFML::VectorMath::getRotatedUnitVector(m_angle) * m_parableParamB;
	sf::Vector2f parable1Pos = m_pos;// -originDir;

	double parable2Fac1 = nan;
	double parable2Fac2 = nan;
	bool parable2Fac1Valid = false;
	bool parable2Fac2Valid = false;
	double parabel2Normal1 = nan;
	double parabel2Normal2 = nan;

	double parable2Rotation = m_angle + M_PI_2;
	sf::Vector2f parable2Pos = m_pos;// +originDir;

	bool c1 = getParableCollisionFactor(ray.ray, m_parableParamA, m_parableParamB, parable1Rotation, parable1Pos, m_diameter / 2,
										parable1Fac1, parable1Fac2, 
										parabel1Normal1, parabel1Normal2,
										parable1Fac1Valid, parable1Fac2Valid);

	bool c2 = getParableCollisionFactor(ray.ray, m_parableParamA, m_parableParamB, parable2Rotation, parable2Pos, m_diameter / 2,
										parable2Fac1, parable2Fac2, 
										parabel2Normal1, parabel2Normal2,
									    parable2Fac1Valid, parable2Fac2Valid);
	//parable1Fac1 = nan;
	//parable1Fac2 = nan;
	//c1 = false;
	if (!(c1 + c2))
		return false;


	float cpyFac11 = parable1Fac1;
	float cpyFac12 = parable1Fac2;
	float cpyFac21 = parable2Fac1;
	float cpyFac22 = parable2Fac2;
	
	if (!parable1Fac1Valid || !c1 || parable1Fac1 < 0) parable1Fac1 = nan;
	if (!parable1Fac2Valid || !c1 || parable1Fac2 < 0) parable1Fac2 = nan;
	if (!parable2Fac1Valid || !c2 || parable2Fac1 < 0) parable2Fac1 = nan;
	if (!parable2Fac2Valid || !c2 || parable2Fac2 < 0) parable2Fac2 = nan;

	double minFac = parable1Fac1;
	double otherFac = cpyFac12;
	double normal = parabel1Normal1;

	if (parable1Fac2 < minFac)
	{
		minFac = parable1Fac2;
		otherFac = cpyFac11;
		normal = parabel1Normal2;
	}else if (parable2Fac1 < minFac)
	{
		minFac = parable2Fac1;
		otherFac = cpyFac22;
		normal = parabel2Normal1;
	}else if (parable2Fac2 < minFac)
	{
		minFac = parable2Fac2;
		otherFac = cpyFac21;
		normal = parabel2Normal2;
	}
	if (minFac >= nan)
		return false;

	outCollisionFactor = minFac;
	outNormalAngle = normal;
	if ((otherFac < 0 && minFac > 0) || (otherFac > 0 && minFac < 0))
	{
		rayStartsInsideShape = true;
		//normal = M_PI - m_lastNormal;
	}
	else
	{
		rayStartsInsideShape = false;
		//m_lastNormal = normal;
	}

	return true;
}

bool ConvexLens::LensShape::getParableCollisionFactor(const QSFML::Utilities::Ray& ray, double a, double b, double parableRotation,
	const sf::Vector2f& parablePos, double minMaxInputRange, 
	double& outFac1, double& outFac2, 
	double& outNormal1, double& outNormal2,
	bool& fac1InRange, bool& fac2InRange) const
{
	/*
		( (bdy ^ 2 - 4 * a * b * bdx ^ 2 + 4 * a * bdx ^ 2 * bpy - 4 * a * bdx * bdy * bpx) ^ (1 / 2) + bdy - 2 * a * bdx * bpx)  / (2 * a * bdx ^ 2);
		-((bdy ^ 2 - 4 * a * b * bdx ^ 2 + 4 * a * bdx ^ 2 * bpy - 4 * a * bdx * bdy * bpx) ^ (1 / 2) - bdy + 2 * a * bdx * bpx) / (2 * a * bdx ^ 2)


		t =  ((bdy^2 - 4*a*b*bdx^2 + 4*a*bdx^2*bpy - 4*a*bdx*bdy*bpx)^(1/2) + bdy - 2*a*bdx*bpx)/(2*a*bdx^2);
			(-(bdy^2 - 4*a*b*bdx^2 + 4*a*bdx^2*bpy - 4*a*bdx*bdy*bpx)^(1/2) + bdy - 2*a*bdx*bpx)/(2*a*bdx^2)
		*/
	QSFML::VectorMath::Vector2d rayPosRaw((double)ray.getPos().x - (double)parablePos.x, (double)ray.getPos().y - (double)parablePos.y);
	QSFML::VectorMath::Vector2d rayDirRaw(ray.getDirection().x, ray.getDirection().y);


	QSFML::VectorMath::Vector2d rayPos = QSFML::VectorMath::getRotated(rayPosRaw, -parableRotation);
	QSFML::VectorMath::Vector2d rayDir = QSFML::VectorMath::getRotated(rayDirRaw, -parableRotation);

	//sf::Vector2f rayPos = QSFML::VectorMath::getRotated(ray.getPos() - parablePos, -parableRotation);
	//sf::Vector2f rayDir = QSFML::VectorMath::getRotated(ray.getDirection(), -parableRotation);

	// Calculations in double precission because when the ray is vertical. it is high volatile
	double bpx = rayPos.x;
	double bpy = rayPos.y;

	double bdx = rayDir.x;
	double bdy = rayDir.y;

	double bdx2 = bdx * bdx;
	double bdy2 = bdy * bdy;

	double diskrim = bdy2 - 4 * a * b * bdx2 + 4 * a * bdx2 * bpy - 4 * a * bdx * bdy * bpx;

	if (diskrim < 0)
	{
		// No collision
		return false;
	}
	double divisor = 2 * a * bdx2;
	double x1;
	double x2;
	if (std::abs(bdx2) < 0.1e-10)
	{
		// vertical collision
		//return false;
		double dirLength = QSFML::VectorMath::getLength(rayDir);


		x1 = bpx;
		x2 = bpx;

		outFac1 = ((a * x1 * x1 + b) - rayPos.y) / dirLength;
		outFac2 = outFac1;

		
		
		outNormal1 = M_PI_2 + parableRotation;
		outNormal2 = M_PI_2 + parableRotation;
		outNormal1 = QSFML::VectorMath::getAngle(QSFML::VectorMath::Vector2d(1, (2 * a * x1))) + M_PI_2 + parableRotation;
		outNormal2 = QSFML::VectorMath::getAngle(QSFML::VectorMath::Vector2d(1, (2 * a * x2))) + M_PI_2 + parableRotation;
	}
	else
	{
		divisor = 1 / divisor;

		double sqrRoot = sqrt(diskrim);
		double adder = bdy - 2 * a * bdx * bpx;

		outFac1 = (sqrRoot + adder) * divisor;
		outFac2 = (-sqrRoot + adder) * divisor;

		x1 = bpx + outFac1 * bdx;
		x2 = bpx + outFac2 * bdx;

		//outNormal1 = QSFML::VectorMath::getAngle(sf::Vector2f(x1, 2 * a * x1)) - M_PI_2 + parableRotation;
		outNormal1 = QSFML::VectorMath::getAngle(QSFML::VectorMath::Vector2d(1, (2 * a * x1))) + M_PI_2 + parableRotation;
		outNormal2 = QSFML::VectorMath::getAngle(QSFML::VectorMath::Vector2d(1, (2 * a * x2))) + M_PI_2 + parableRotation;
	}
	
	

	

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

	/*if (fac1InRange)
	{
		sf::CircleShape* c1 = new sf::CircleShape(1, 10);
		c1->setOrigin(1, 1);
		c1->setPosition(ray.getPoint(outFac1));
		m_tmpDraw.push_back(c1);
	}
	if (fac2InRange)
	{
		sf::CircleShape* c1 = new sf::CircleShape(1, 10);
		c1->setOrigin(1, 1);
		c1->setPosition(ray.getPoint(outFac2));
		m_tmpDraw.push_back(c1);
	}*/
	return true;


}

void ConvexLens::LensShape::getPainterVertecies(std::vector < sf::Vertex>& left, std::vector < sf::Vertex>& right, size_t resolution)
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



bool ConvexLens::LensShape::getCollisionData(const LightRay& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{

	
#ifdef USE_CIRCLE
	float fac = -1;
	float normalAngle;
	bool isInside;
	if (getLensCollisionFactor(ray, false, fac, normalAngle, isInside))
	{
		outCollisionFactor = fac;
		outNormalAngle = normalAngle;
		rayStartsInsideShape = isInside;
		return true;
	}
	return false;
#endif
#ifdef USE_PARABLE
	return getParableData(ray, outCollisionFactor, outNormalAngle, rayStartsInsideShape);
#endif
}
