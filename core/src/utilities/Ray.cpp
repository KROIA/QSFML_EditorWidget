#include "utilities/Ray.h"
#include "utilities/VectorOperations.h"
#include "components/drawable/Shape.h"

#include <SFML/OpenGL.hpp>

namespace QSFML
{
	namespace Utilities
	{
		Ray::Ray()
			: Transformable()
			, m_dir(1,0)
			, m_dirLength(1)
			, m_rayPainter(nullptr)
		{

		}
		Ray::Ray(const Ray& other)
			: Transformable(other)
			, m_dir(other.m_dir)
			, m_dirLength(other.m_dirLength)
			, m_rayPainter(nullptr)
		{
			
		}
		Ray::Ray(const sf::Vector2f& position, const sf::Vector2f& direction)
			: Transformable(position, 0)
			, m_dir(direction)
			, m_dirLength(sqrt(direction.x * direction.x + direction.y * direction.y))
			, m_rayPainter(nullptr)
		{

		}
		Ray::Ray(float posX, float posY, float dirX, float dirY)
			: Transformable({ posX, posY }, 0)
			, m_dir(dirX, dirY)
			, m_dirLength(sqrt(dirX * dirX + dirY * dirY))
			, m_rayPainter(nullptr)
		{

		}

		Ray::~Ray()
		{
			if (m_rayPainter)
			{
				m_rayPainter->disconnect_onDestroy(this, &Ray::onRayPainterDestroyed);
				m_rayPainter = nullptr;
			}
		}

		Ray& Ray::operator=(const Ray& other)
		{
			m_dir = other.m_dir;
			m_dirLength = other.m_dirLength;
			Transformable::operator=(other);
			return *this;
		}
		bool Ray::operator==(const Ray& other) const
		{
			if (m_dirLength != other.m_dirLength)
				return false;
			if (Transformable::operator==(other) == false)
				return false;
			if (m_dir.x - other.m_dir.x != 0)
				return false;
			if (m_dir.y - other.m_dir.y != 0)
				return false;
			return true;
		}
		bool Ray::operator!=(const Ray& other) const
		{
			if (m_dirLength != other.m_dirLength)
				return true;
			if (Transformable::operator!=(other) == false)
				return true;
			if (m_dir.x - other.m_dir.x != 0)
				return true;
			if (m_dir.y - other.m_dir.y != 0)
				return true;
			return false;
		}

		Ray Ray::operator+(const sf::Vector2f& offset) const
		{
			return Ray(getPosition() + offset, m_dir);
		}
		Ray Ray::operator-(const sf::Vector2f & offset) const
		{
			return Ray(getPosition() - offset, m_dir);
		}
		Ray& Ray::operator+=(const sf::Vector2f& offset)
		{
			move(offset);
			return *this;
		}
		Ray& Ray::operator-=(const sf::Vector2f& offset)
		{
			move(-offset);
			return *this;
		}
		Ray Ray::operator*(float factor) const
		{
			return Ray(getPosition(), m_dir * factor);
		}
		Ray Ray::operator/(float divisor) const
		{
			return Ray(getPosition(), m_dir / divisor);
		}
		Ray& Ray::operator*=(float factor)
		{
			m_dir *= factor;
			m_dirLength *= factor;
			return *this;
		}
		Ray& Ray::operator/=(float divisor)
		{
			m_dir /= divisor;
			m_dirLength /= divisor;
			return *this;
		}

		Ray Ray::getNormalized() const
		{
			if (m_dirLength == 0)
				return Ray();
			return Ray(getPosition(), m_dir / m_dirLength);
		}
		void Ray::normalize()
		{
			if (m_dirLength == 0)
				m_dir = sf::Vector2f(1, 0);
			else
				m_dir /= m_dirLength;
			m_dirLength = 1;
		}

		/*void Ray::setPos(const sf::Vector2f& pos)
		{
			getPosition() = pos;
		}
		void Ray::setPos(float posX, float posY)
		{
			getPosition().x = posX;
			getPosition().y = posY;
		}
		const sf::Vector2f& Ray::getPos() const
		{
			return getPosition();
		}*/

		void Ray::setDirection(const sf::Vector2f& dir)
		{
			m_dir = dir;
			m_dirLength = sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
		}
		void Ray::setDirection(float dirX, float dirY)
		{
			m_dir.x = dirX;
			m_dir.y = dirY;
			m_dirLength = sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
		}
		const sf::Vector2f& Ray::getDirection() const
		{
			return m_dir;
		}
		float Ray::getAngle() const
		{
			return VectorMath::getAngle(m_dir);
		}
		float Ray::getAngle(const Ray& ray) const
		{
			return VectorMath::getAngle(m_dir, ray.m_dir);
		}

		sf::Vector2f Ray::getPoint(float scalar) const
		{
			return getPosition() + scalar * m_dir;
		}
		float Ray::getShortestDistanceFactor(const sf::Vector2f& point, bool* failed) const
		{
			if (m_dirLength == 0)
			{
				if (failed)
					*failed = true;
				return 0;
			}
			float divisor = m_dir.x * m_dir.x + m_dir.y * m_dir.y;
			if (failed)
				*failed = false;
			return -(getPosition().x * m_dir.x + getPosition().y * m_dir.y - m_dir.x * point.x - m_dir.y * point.y) / divisor;
		}
		float Ray::getShortestDistance(const sf::Vector2f& point, bool* failed) const
		{
			if (m_dirLength == 0)
			{
				if (failed)
					*failed = true;
				return 0;
			}
			if (failed)
				*failed = false;
			return abs(getPosition().x * m_dir.y - getPosition().y * m_dir.x + m_dir.x * point.y - m_dir.y * point.x) / m_dirLength;
		}

		bool Ray::raycast(const sf::Vector2f& pointA,
										   const sf::Vector2f& directionA,
										   const sf::Vector2f& pointB,
										   const sf::Vector2f& directionB,
										   float& outDistanceFactorA,
										   float& outDistanceFactorB)
		{
			float divisor = directionA.x * directionB.y - directionA.y * directionB.x;


			if (std::abs(divisor) < 1e-6)
			{
				outDistanceFactorA = NAN;
				outDistanceFactorB = NAN;
				return false;
			}
			divisor = 1.f / divisor;
			float aNumerator = -(pointA.x * directionB.y - pointA.y * directionB.x - pointB.x * directionB.y + pointB.y * directionB.x);
			float bNumerator = -(pointA.x * directionA.y - pointA.y * directionA.x + directionA.x * pointB.y - directionA.y * pointB.x);

			outDistanceFactorA = aNumerator * divisor;
			outDistanceFactorB = bNumerator * divisor;
			return true;
		}

		bool Ray::raycast(const Ray& other,
						  float& outDistanceFactorA,
						  float& outDistanceFactorB) const
		{
			bool ret = raycast(getPosition(), m_dir, other.getPosition(), other.m_dir, outDistanceFactorA, outDistanceFactorB);
			if(ret && m_rayPainter)
			{
				sf::Vector2f pointA = getPoint(outDistanceFactorA);
				sf::Vector2f pointB = other.getPoint(outDistanceFactorB);
				m_rayPainter->addPoint(pointA);
				m_rayPainter->addPoint(pointB);
				m_rayPainter->addLine(getPosition(), pointA);
				m_rayPainter->addLine(other.getPosition(), pointB);
			}
			return ret;
		}

		int Ray::getCircleCollisionFactors(const sf::Vector2f& circlePos, float circleRadius,
										   float& outFactor1, float& outFactor2) const
		{
			bool failed;
			float shortestFactorToCenter = getShortestDistanceFactor(circlePos, &failed);
			QSFML_UNUSED(shortestFactorToCenter);
			if (failed)
				return 0; // No collision on circle

			float m_dirx2 = m_dir.x * m_dir.x;
			float m_diry2 = m_dir.y * m_dir.y;

			float m_posy2 = getPosition().y * getPosition().y;
			float m_posx2 = getPosition().x * getPosition().x;

			float xp2 = circlePos.x * circlePos.x;
			float yp2 = circlePos.y * circlePos.y;
			float r2 = circleRadius * circleRadius;

			float m_posx_xp = getPosition().x - circlePos.x;
			float m_posy_yp = getPosition().y - circlePos.y;

			float divisor = (m_dirx2 + m_diry2);
			float diskrim = divisor * r2 - m_dirx2 * (m_posy2 - 2 * getPosition().y * circlePos.y + yp2) + 2 * m_dir.x * m_dir.y * m_posx_xp * m_posy_yp-m_diry2 * (m_posx2 - 2 * getPosition().x * circlePos.x + xp2);
			
			float tmp1 = m_dir.x * (m_posx_xp)+m_dir.y * (m_posy_yp);
			float root = sqrt(diskrim);
			float t1 = ((root - tmp1) / divisor);
			float t2 = -((root + tmp1) / divisor);

			outFactor1 = t1;
			outFactor2 = t2;
			if (diskrim == 0)
				return 1;
			return 2;
		}

		bool Ray::raycast(const Components::Shape& shape, float& outDistanceFactor, size_t& outEdge) const
		{
			bool ret = raycast_internal(shape, outDistanceFactor, outEdge);
			if (ret && m_rayPainter)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_rayPainter->addPoint(point);
				m_rayPainter->addLine(getPosition(), point);
			}
			return ret;
		}
		bool Ray::raycast(const std::vector<Components::Shape>& shapes, 
			float& outDistanceFactor, 
			size_t& outShapeIndex, 
			size_t& outEdge) const
		{
			float minDistance = std::numeric_limits<float>::max();
			bool ret = false;
			for (size_t i = 0; i < shapes.size(); ++i)
			{
				float distanceFactor;
				size_t edge;
				if (raycast(shapes[i], distanceFactor, edge))
				{
					if (distanceFactor < minDistance)
					{
						minDistance = distanceFactor;
						outShapeIndex = i;
						outEdge = edge;
						ret = true;
					}
				}
			}
			if (ret && m_rayPainter)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_rayPainter->addPoint(point);
				m_rayPainter->addLine(getPosition(), point);
			}
			return ret;
		}
		bool Ray::raycast(const AABB& aabb, float& outDistanceFactor, size_t& outEdge) const
		{
			bool ret = raycast_internal(aabb, outDistanceFactor, outEdge);
			if (ret && m_rayPainter)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_rayPainter->addPoint(point);
				m_rayPainter->addLine(getPosition(), point);
			}
			return ret;
		}
		bool Ray::raycast_internal(const Components::Shape& shape, float& outDistanceFactor, size_t& outEdge) const
		{
			Utilities::AABB aabb = shape.getGlobalBounds();
			float dummy1;
			size_t edge;
			if (!raycast_internal(aabb, dummy1, edge))
				return false;

			std::vector<sf::Vector2f> points = shape.getTransformedPoints();

			float minDistance = std::numeric_limits<float>::max();
			for (size_t i = 0; i < points.size(); ++i)
			{
				const sf::Vector2f& pointA = points[i];
				const sf::Vector2f& pointB = points[(i + 1) % points.size()];
				sf::Vector2f direction = pointB - pointA;
				float distanceFactor;
				float dummy;
				if (raycast(getPosition(), m_dir, pointA, direction, distanceFactor, dummy))
				{
					if (distanceFactor < 0 || dummy < 0 || dummy > 1)
						continue;

					if (distanceFactor < minDistance)
					{
						outEdge = i;
						minDistance = distanceFactor;
					}
				}
			}
			if (minDistance < std::numeric_limits<float>::max() - 1)
			{
				outDistanceFactor = minDistance;
				return true;
			}
			return false;
		}
		bool Ray::raycast_internal(const AABB& aabb, float& outDistanceFactor, size_t& outEdge) const
		{
			float minDistance = std::numeric_limits<float>::max();

			sf::Vector2f points[4] = { aabb.TL(), aabb.TR(), aabb.BR(), aabb .BL()};

			for (size_t i = 0; i < 4; ++i)
			{
				const sf::Vector2f &pointA = points[i];
				const sf::Vector2f &pointB = points[(i+1) % 4];
				sf::Vector2f direction = pointB - pointA;
				float distanceFactor;
				float dummy;
				if (raycast(getPosition(), m_dir, pointA, direction, distanceFactor, dummy))
				{
					if (distanceFactor < 0 || dummy < 0 || dummy > 1)
						continue;
					
					if (distanceFactor < minDistance)
					{
						outEdge = i;
						minDistance = distanceFactor;
					}
				}
			}
			if (minDistance < std::numeric_limits<float>::max() - 1)
			{
				outDistanceFactor = minDistance;
				return true;
			}
			return false;
		}

		Ray::RayPainter* Ray::createRayPainter()
		{
			if (m_rayPainter)
				return m_rayPainter;
			m_rayPainter = new Ray::RayPainter();
			m_rayPainter->connect_onDestroy(this, &Ray::onRayPainterDestroyed);
			return m_rayPainter;
		}
		void Ray::onRayPainterDestroyed()
		{
			m_rayPainter = nullptr;
		}


		Ray::RayPainter::RayPainter(const std::string& name)
			: Drawable(name)
			, m_pointColor(sf::Color::Red)
			, m_lineColor(sf::Color::White)
			, m_pointRadius(2)
		{

		}
		
		Ray::RayPainter::~RayPainter()
		{

		}

		void Ray::RayPainter::addPoint(const sf::Vector2f& point)
		{
			m_points.push_back(point);
		}
		void Ray::RayPainter::addLine(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
		{
			LinePainter line;
			line.line[0] = pointA;
			line.line[1] = pointB;

			m_lines.push_back(line);
		}

		void Ray::RayPainter::drawComponent(sf::RenderTarget& target, sf::RenderStates states) const
		{
			QSFML_UNUSED(states);
#ifdef QSFML_USE_GL_DRAW
			QSFML_UNUSED(target);
			glBegin(GL_LINES);
			glColor4ub(m_lineColor.r, m_lineColor.g, m_lineColor.b, m_lineColor.a);
			for (size_t i = 0; i < m_lines.size(); ++i)
			{
				const LinePainter& line = m_lines[i];
				
				glVertex2f(line.line[0].x, line.line[0].y);
				//glColor4ub(line.m_line[1].color.r, line.m_line[1].color.g, line.m_line[1].color.b, line.m_line[1].color.a);
				glVertex2f(line.line[1].x, line.line[1].y);
			}
			glEnd();

			glBegin(GL_LINES);
			glColor4ub(m_pointColor.r, m_pointColor.g, m_pointColor.b, m_pointColor.a);
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				const sf::Vector2f& point = m_points[i];
				sf::Vector2f tl = point - sf::Vector2f(m_pointRadius, m_pointRadius);
				sf::Vector2f tr = point + sf::Vector2f(m_pointRadius, -m_pointRadius);
				sf::Vector2f bl = point + sf::Vector2f(-m_pointRadius, m_pointRadius);
				sf::Vector2f br = point + sf::Vector2f(m_pointRadius, m_pointRadius);

				// Draw cross on that point
				glVertex2f(tl.x,tl.y);
				glVertex2f(br.x,br.y);
				glVertex2f(tr.x,tr.y);
				glVertex2f(bl.x,bl.y);


				//glVertex2f(point.x, point.y);
			}
			glEnd();
			m_lines.clear();
			m_points.clear();
#else
			//states.transform = m_ra();
			for (size_t i = 0; i < m_lines.size(); ++i)
			{
				sf::Vertex line[2];
				line[0].position = m_lines[i].line[0];
				line[1].position = m_lines[i].line[1];
				line[0].color = m_lineColor;
				line[1].color = m_lineColor;
				target.draw(line, 2, sf::Lines);
			}
			
			sf::CircleShape point(m_pointRadius);
			point.setFillColor(m_pointColor);
			point.setOrigin(m_pointRadius, m_pointRadius);
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				point.setPosition(m_points[i]);
				target.draw(point);
			}
			m_lines.clear();
			m_points.clear();
#endif
		}

	}
}
