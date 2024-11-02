#include "utilities/Ray.h"
#include "utilities/VectorOperations.h"
#include "components/drawable/Shape.h"

#include <SFML/OpenGL.hpp>

namespace QSFML
{
	namespace Utilities
	{
		sf::Color Ray::s_gizmoLineColor(255, 0, 0, 255);
		sf::Color Ray::s_gizmoPointColor(255,255,255,255);
		float Ray::s_gizmoPointRadius = 10;
		size_t Ray::s_maxGizmoCount = 10;

		Ray::Ray()
			: m_pos(0,0)
			, m_dir(1,0)
			, m_dirLength(1)
		{

		}
		Ray::Ray(const Ray& other)
			: m_pos(other.m_pos)
			, m_dir(other.m_dir)
			, m_dirLength(other.m_dirLength)
		{
			
		}
		Ray::Ray(const sf::Vector2f& position, const sf::Vector2f& direction)
			: m_pos(position)
			, m_dir(direction)
			, m_dirLength(sqrt(direction.x * direction.x + direction.y * direction.y))
		{

		}
		Ray::Ray(float posX, float posY, float dirX, float dirY)
			: m_pos(posX, posY)
			, m_dir(dirX, dirY)
			, m_dirLength(sqrt(dirX * dirX + dirY * dirY))
		{

		}

		Ray::~Ray()
		{

		}

		Ray& Ray::operator=(const Ray& other)
		{
			m_pos = other.m_pos;
			m_dir = other.m_dir;
			m_dirLength = other.m_dirLength;
			return *this;
		}
		bool Ray::operator==(const Ray& other) const
		{
			if (m_dirLength != other.m_dirLength)
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
			if (m_dir.x - other.m_dir.x != 0)
				return true;
			if (m_dir.y - other.m_dir.y != 0)
				return true;
			return false;
		}

		Ray Ray::operator+(const sf::Vector2f& offset) const
		{
			return Ray(m_pos + offset, m_dir);
		}
		Ray Ray::operator-(const sf::Vector2f & offset) const
		{
			return Ray(m_pos - offset, m_dir);
		}
		Ray& Ray::operator+=(const sf::Vector2f& offset)
		{
			m_pos += offset;
			return *this;
		}
		Ray& Ray::operator-=(const sf::Vector2f& offset)
		{
			m_pos -= offset;
			return *this;
		}
		Ray Ray::operator*(float factor) const
		{
			return Ray(m_pos, m_dir * factor);
		}
		Ray Ray::operator/(float divisor) const
		{
			return Ray(m_pos, m_dir / divisor);
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
			return Ray(m_pos, m_dir / m_dirLength);
		}
		void Ray::normalize()
		{
			if (m_dirLength == 0)
				m_dir = sf::Vector2f(1, 0);
			else
				m_dir /= m_dirLength;
			m_dirLength = 1;
		}

		void Ray::setPosition(const sf::Vector2f& pos)
		{
			m_pos = pos;
		}
		void Ray::setPosition(float posX, float posY)
		{
			m_pos.x = posX;
			m_pos.y = posY;
		}
		const sf::Vector2f& Ray::getPosition() const
		{
			return m_pos;
		}

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
		float Ray::getAngleRAD() const
		{
			return VectorMath::getAngleRAD(m_dir);
		}
		float Ray::getAngleRAD(const Ray& ray) const
		{
			return VectorMath::getAngleRAD(m_dir, ray.m_dir);
		}

		sf::Vector2f Ray::getPoint(float scalar) const
		{
			return m_pos + scalar * m_dir;
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
			return -(m_pos.x * m_dir.x + m_pos.y * m_dir.y - m_dir.x * point.x - m_dir.y * point.y) / divisor;
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
			return abs(m_pos.x * m_dir.y - m_pos.y * m_dir.x + m_dir.x * point.y - m_dir.y * point.x) / m_dirLength;
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
			bool ret = raycast(m_pos, m_dir, other.m_pos, other.m_dir, outDistanceFactorA, outDistanceFactorB);
			if(ret && m_gizmoPoints.size() < s_maxGizmoCount)
			{
				sf::Vector2f pointA = getPoint(outDistanceFactorA);
				sf::Vector2f pointB = other.getPoint(outDistanceFactorB);
				m_gizmoPoints.push_back(pointA);
				m_gizmoPoints.push_back(pointB);
				m_gizmoLines.push_back({ m_pos, pointA });
				m_gizmoLines.push_back({ other.m_pos, pointB });
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

			float m_posy2 = m_pos.y * m_pos.y;
			float m_posx2 = m_pos.x * m_pos.x;

			float xp2 = circlePos.x * circlePos.x;
			float yp2 = circlePos.y * circlePos.y;
			float r2 = circleRadius * circleRadius;

			float m_posx_xp = m_pos.x - circlePos.x;
			float m_posy_yp = m_pos.y - circlePos.y;

			float divisor = (m_dirx2 + m_diry2);
			float diskrim = divisor * r2 - m_dirx2 * (m_posy2 - 2 * m_pos.y * circlePos.y + yp2) + 2 * m_dir.x * m_dir.y * m_posx_xp * m_posy_yp-m_diry2 * (m_posx2 - 2 * m_pos.x * circlePos.x + xp2);
			
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
			if (ret && m_gizmoPoints.size() < s_maxGizmoCount)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_gizmoPoints.push_back(point);
				m_gizmoLines.push_back({ m_pos, point });
			}
			return ret;
		}
		bool Ray::raycast(const QSFML::vector<Components::Shape>& shapes, 
			float& outDistanceFactor, 
			size_t& outShapeIndex, 
			size_t& outEdge) const
		{
			outDistanceFactor = std::numeric_limits<float>::max();
			bool ret = false;
			for (size_t i = 0; i < shapes.size(); ++i)
			{
				float distanceFactor;
				size_t edge;
				if (raycast_internal(shapes[i], distanceFactor, edge))
				{
					if (distanceFactor < outDistanceFactor)
					{
						outDistanceFactor = distanceFactor;
						outShapeIndex = i;
						outEdge = edge;
						ret = true;
					}
				}
			}
			if (ret && m_gizmoPoints.size() < s_maxGizmoCount)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_gizmoPoints.push_back(point);
				m_gizmoLines.push_back({ m_pos, point });
			}
			return ret;
		}
		bool Ray::raycast(const QSFML::vector<Components::Shape*>& shapes,
			float& outDistanceFactor,
			size_t& outShapeIndex,
			size_t& outEdge) const
		{
			outDistanceFactor = std::numeric_limits<float>::max();
			bool ret = false;
			for (size_t i = 0; i < shapes.size(); ++i)
			{
				float distanceFactor;
				size_t edge;
				if (raycast_internal(*shapes[i], distanceFactor, edge))
				{
					if (distanceFactor < outDistanceFactor)
					{
						outDistanceFactor = distanceFactor;
						outShapeIndex = i;
						outEdge = edge;
						ret = true;
					}
				}
			}
			if (ret && m_gizmoPoints.size() < s_maxGizmoCount)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_gizmoPoints.push_back(point);
				m_gizmoLines.push_back({ m_pos, point });
			}
			return ret;
		}
		bool Ray::raycast(const AABB& aabb, float& outDistanceFactor, size_t& outEdge) const
		{
			bool ret = raycast_internal(aabb, outDistanceFactor, outEdge);
			if (ret && m_gizmoPoints.size() < s_maxGizmoCount)
			{
				sf::Vector2f point = getPoint(outDistanceFactor);
				m_gizmoPoints.push_back(point);
				m_gizmoLines.push_back({ m_pos, point });
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

			QSFML::vector<sf::Vector2f> points = shape.getTransformedPoints();

			float minDistance = std::numeric_limits<float>::max();
			for (size_t i = 0; i < points.size(); ++i)
			{
				const sf::Vector2f& pointA = points[i];
				const sf::Vector2f& pointB = points[(i + 1) % points.size()];
				sf::Vector2f direction = pointB - pointA;
				float distanceFactor;
				float dummy;
				if (raycast(m_pos, m_dir, pointA, direction, distanceFactor, dummy))
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
				if (raycast(m_pos, m_dir, pointA, direction, distanceFactor, dummy))
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

		void Ray::drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const
		{
			QSFML_UNUSED(states);
#ifdef QSFML_USE_GL_DRAW
			QSFML_UNUSED(target);
			glLoadMatrixf(sf::Transform::Identity.getMatrix());
			glLineWidth(1);
			glBegin(GL_LINES);
			glColor4ub(s_gizmoLineColor.r, s_gizmoLineColor.g, s_gizmoLineColor.b, s_gizmoLineColor.a);
			for (size_t i = 0; i < m_gizmoLines.size(); ++i)
			{
				const auto& line = m_gizmoLines[i];
				glVertex2f(line.first.x, line.first.y);
				glVertex2f(line.second.x, line.second.y);
			}
			glEnd();

			glBegin(GL_LINES);
			glColor4ub(s_gizmoPointColor.r, s_gizmoPointColor.g, s_gizmoPointColor.b, s_gizmoPointColor.a);
			for (size_t i = 0; i < m_gizmoPoints.size(); ++i)
			{
				const sf::Vector2f& point = m_gizmoPoints[i];
				sf::Vector2f tl = point - sf::Vector2f(s_gizmoPointRadius, s_gizmoPointRadius);
				sf::Vector2f tr = point + sf::Vector2f(s_gizmoPointRadius, -s_gizmoPointRadius);
				sf::Vector2f bl = point + sf::Vector2f(-s_gizmoPointRadius, s_gizmoPointRadius);
				sf::Vector2f br = point + sf::Vector2f(s_gizmoPointRadius, s_gizmoPointRadius);

				// Draw cross on that point
				glVertex2f(tl.x, tl.y);
				glVertex2f(br.x, br.y);
				glVertex2f(tr.x, tr.y);
				glVertex2f(bl.x, bl.y);


				//glVertex2f(point.x, point.y);
			}
			glEnd();
			
#else
			//states.transform = m_ra();
			sf::Vertex line[2];
			line[0].color = s_gizmoLineColor;
			line[1].color = s_gizmoLineColor;
			for (size_t i = 0; i < m_gizmoLines.size(); ++i)
			{
				line[0].position = m_gizmoLines[i].first;
				line[1].position = m_gizmoLines[i].second;
				target.draw(line, 2, sf::Lines);
			}

			line[0].color = s_gizmoPointColor;
			line[1].color = s_gizmoPointColor;
			
			for (size_t i = 0; i < m_gizmoPoints.size(); ++i)
			{
				const sf::Vector2f& point = m_gizmoPoints[i];
				sf::Vector2f tl = point - sf::Vector2f(s_gizmoPointRadius, s_gizmoPointRadius);
				sf::Vector2f tr = point + sf::Vector2f(s_gizmoPointRadius, -s_gizmoPointRadius);
				sf::Vector2f bl = point + sf::Vector2f(-s_gizmoPointRadius, s_gizmoPointRadius);
				sf::Vector2f br = point + sf::Vector2f(s_gizmoPointRadius, s_gizmoPointRadius);

				line[0].position = tl;
				line[1].position = br;
				target.draw(line, 2, sf::Lines);

				line[0].position = tr;
				line[1].position = bl;
				target.draw(line, 2, sf::Lines);
			}
#endif
			m_gizmoLines.clear();
			m_gizmoPoints.clear();
			m_gizmoLines.reserve(s_maxGizmoCount);
			m_gizmoPoints.reserve(s_maxGizmoCount);
		}
	}
}
