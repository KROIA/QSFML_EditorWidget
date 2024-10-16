#include "SimpleLens.h"
#include <qpainter.h>

//#define USE_CIRCLE
#define USE_PARABLE
//#define USE_HYPERBOL

SimpleLens::SimpleLens(const std::string& name)
	: OpticalElement(name)
{
	m_shape.m_painter = new LensShape::LensPainter(&m_shape);
	m_shape.m_diameter = 30;
	m_shape.m_focusLength = 50;
	m_shape.m_angle = M_PI / 4;
	m_shape.m_pos = sf::Vector2f(0, 0);
	m_shape.m_lens = this;
	addComponent(m_shape.m_painter);
	setShape(&m_shape);
}
SimpleLens::SimpleLens(const SimpleLens& other)
	: OpticalElement(other)
{
	m_shape.m_painter = new LensShape::LensPainter(&m_shape);
	m_shape.m_diameter = other.m_shape.m_diameter;
	m_shape.m_focusLength = other.m_shape.m_focusLength;
	m_shape.m_angle = other.m_shape.m_angle;
	m_shape.m_pos = other.m_shape.m_pos;
	m_shape.m_lens = this;
	addComponent(m_shape.m_painter);
	setShape(&m_shape);
}
SimpleLens::~SimpleLens()
{

}
OBJECT_IMPL(SimpleLens);


void SimpleLens::setColor(const sf::Color& c)
{
	m_shape.m_painter->m_color = c;
}
const sf::Color& SimpleLens::getColor() const
{
	return m_shape.m_painter->m_color;
}

void SimpleLens::setPos(const sf::Vector2f& pos)
{
	m_shape.m_pos = pos;
}
const sf::Vector2f& SimpleLens::getPos() const
{
	return m_shape.m_pos;
}

void SimpleLens::setRotation(float angle)
{
	m_shape.m_angle = angle;
}
float SimpleLens::getRotation() const
{
	return m_shape.m_angle;
}
void SimpleLens::setDiameter(float w)
{
	m_shape.m_diameter = abs(w);
}
float SimpleLens::getDiameter() const
{
	return m_shape.m_diameter;
}
void SimpleLens::setFocusLength(float r)
{
	m_shape.m_focusLength = r;
}
float SimpleLens::getFocusLength() const
{
	return m_shape.getFocusLength();
}


/*
bool SimpleLens::processLaser(const QSFML::Utilities::Ray& ray,
	QSFML::vector< QSFML::Utilities::Ray>& reflectedOut,
	QSFML::vector< LaserInfo>& additionalLightPathsOut) const
{
	sf::Vector2f collisionPoint;
	return processLaser_intern(ray, reflectedOut, additionalLightPathsOut, false, collisionPoint);
}

bool SimpleLens::getRaycastDistance(const QSFML::Utilities::Ray& ray, float& distanceOut) const
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

bool SimpleLens::processLaser_intern(const QSFML::Utilities::Ray& ray,
	QSFML::vector< QSFML::Utilities::Ray>& reflectedOut,
	QSFML::vector< LaserInfo>& additionalLightPathsOut, bool outgoingRay,
	sf::Vector2f& collisionPointOut) const
{


	float fac = -1;
	float normalAngle;
	if (getLensCollisionFactor(ray, outgoingRay, fac, normalAngle))
	{
		collisionPointOut = ray.getPoint(fac);
		float rayAngle = QSFML::VectorMath::getAngleRAD(ray.getDirection());


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
			sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVectorRAD(data.reflectAngle);
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
			sf::Vector2f dir = QSFML::VectorMath::getRotatedUnitVectorRAD(data.refractAngle);

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


void SimpleLens::update()
{
	//setRotation(getRotation() + 0.01);
}

void SimpleLens::reflectAndRefract(float rayAngle, float normalAngle, float n1, float n2,
	ReflectionAndRefractionData& outData) const
{
	rayAngle = QSFML::VectorMath::getNormalzedAngleRAD(rayAngle);
	normalAngle = QSFML::VectorMath::getNormalzedAngleRAD(normalAngle);

	// Berechne reflexion
	outData.reflectAngle = normalAngle;
	outData.refractAngle = normalAngle;
	outData.doesRefract = true;
}


SimpleLens::LensShape::LensPainter::LensPainter(LensShape* lense, const std::string& name)
	: Drawable(name)
	, m_lens(lense)
{

}
SimpleLens::LensShape::LensPainter::LensPainter(const LensPainter& other)
	: Drawable(other)
	, m_lens(nullptr)
{

}
COMPONENT_IMPL(SimpleLens::LensShape::LensPainter);


void SimpleLens::LensShape::LensPainter::drawComponent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	const size_t resolution = 20;
	QSFML::vector < sf::Vertex> points;
	m_lens->getPainterVertecies(points);

	for (size_t i = 0; i < points.size(); ++i)
	{
		points[i].color = m_color;
	}

	target.draw(points.data(), points.size(), sf::LineStrip);

	for (size_t i = 0; i < m_lens->m_tmpDraw.size(); ++i)
	{
		target.draw(*m_lens->m_tmpDraw[i]);
		delete m_lens->m_tmpDraw[i];
	}
	m_lens->m_tmpDraw.clear();
}


float  SimpleLens::LensShape::getFocusLength() const
{
	return m_focusLength;
}


void SimpleLens::LensShape::getPainterVertecies(QSFML::vector < sf::Vertex>& points)
{
	points = QSFML::vector < sf::Vertex>
	{
		sf::Vertex(m_pos + QSFML::VectorMath::getRotatedUnitVectorRAD(m_angle)*m_diameter / 2.f),
		sf::Vertex(m_pos - QSFML::VectorMath::getRotatedUnitVectorRAD(m_angle)*m_diameter / 2.f)
	};
}



bool SimpleLens::LensShape::getCollisionData(const LightRay& ray,
	float& outCollisionFactor, float& outNormalAngle, bool& rayStartsInsideShape) const
{

	QSFML::Utilities::Ray shapeRay(m_pos, QSFML::VectorMath::getRotatedUnitVectorRAD(m_angle) * m_diameter / 2.f);

	float fac1 = 99999999;
	float fac2 = 99999999;
	shapeRay.raycast(ray.ray, fac1, fac2);
	if (fac1 >= 1 || fac1 <= -1 || fac2 < 0)
		return false;
	outCollisionFactor = fac2;
	//outNormalAngle = m_angle + M_PI_2;
	rayStartsInsideShape = false;

	sf::Vector2f collisionPoint = ray.ray.getPoint(fac2);
	float rayAngle = QSFML::VectorMath::getAngleRAD(ray.ray.getDirection());
	if (rayAngle - m_angle < M_PI_2)
	{
		sf::Vector2f focusPoint = m_pos + (QSFML::VectorMath::getRotatedUnitVectorRAD((float)(m_angle-M_PI_2)) * m_focusLength);
		float focusAngle = QSFML::VectorMath::getAngleRAD(focusPoint - collisionPoint);
		outNormalAngle = focusAngle;
	}
	else
	{
		sf::Vector2f focusPoint = m_pos + (QSFML::VectorMath::getRotatedUnitVectorRAD((float)(m_angle + M_PI_2)) * m_focusLength);
		float focusAngle = QSFML::VectorMath::getAngleRAD(focusPoint - collisionPoint);
		outNormalAngle = focusAngle;
	}

	return true;
}
