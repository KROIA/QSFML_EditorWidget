#include "utilities/Ray.h"


namespace QSFML
{
	namespace Utilities
	{
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
			if (m_pos.x - other.m_pos.x != 0)
				return false;
			if (m_pos.y - other.m_pos.y != 0)
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
			if (m_pos.x - other.m_pos.x != 0)
				return true;
			if (m_pos.y - other.m_pos.y != 0)
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

		void Ray::setPos(const sf::Vector2f& pos)
		{
			m_pos = pos;
		}
		void Ray::setPos(float posX, float posY)
		{
			m_pos.x = posX;
			m_pos.y = posY;
		}
		const sf::Vector2f& Ray::getPos() const
		{
			return m_pos;
		}

		void Ray::setDir(const sf::Vector2f& dir)
		{
			m_dir = dir;
			m_dirLength = sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
		}
		void Ray::setDir(float dirX, float dirY)
		{
			m_dir.x = dirX;
			m_dir.y = dirY;
			m_dirLength = sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
		}
		const sf::Vector2f& Ray::getDir() const
		{
			return m_dir;
		}

		sf::Vector2f Ray::getPoint(float scalar)
		{
			return m_pos + scalar * m_dir;
		}
		float Ray::getShortestDistanceFactor(const sf::Vector2f& point, bool* failed)
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
		float Ray::getShortestDistance(const sf::Vector2f& point, bool* failed)
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
			return raycast(m_pos, m_dir, other.m_pos, other.m_dir, outDistanceFactorA, outDistanceFactorB);
		}

	}
}