#pragma once

#include "QSFML_EditorWidget_base.h"
#include "components/base/Drawable.h"
#include "utilities/Transformable.h"

namespace QSFML
{
	namespace Components
	{
		class Shape;
	}
	namespace Utilities
	{
		class AABB;
		class QSFML_EDITOR_WIDGET_EXPORT Ray //: public Utilities::Transformable
		{
			friend class RayPainter;
		public:
			static void setGizmoLineColor(const sf::Color& color)
			{
				s_gizmoLineColor = color;
			}
			static void setGizmoPointColor(const sf::Color& color)
			{
				s_gizmoPointColor = color;
			}
			static void setGizmoPointRadius(float radius)
			{
				s_gizmoPointRadius = radius;
			}
			static void setMaxGizmoCount(size_t count)
			{
				s_maxGizmoCount = count;
			}
			static const sf::Color& getGizmoLineColor()
			{
				return s_gizmoLineColor;
			}
			static const sf::Color& getGizmoPointColor()
			{
				return s_gizmoPointColor;
			}
			static float getGizmoPointRadius()
			{
				return s_gizmoPointRadius;
			}
			static size_t getMaxGizmoCount()
			{
				return s_maxGizmoCount;
			}


			Ray();
			Ray(const Ray& other);
			Ray(const sf::Vector2f& position, const sf::Vector2f& direction);
			Ray(float posX, float posY, float dirX, float dirY);
			~Ray();

			Ray& operator=(const Ray& other);
			bool operator==(const Ray& other) const;
			bool operator!=(const Ray& other) const;

			Ray operator+(const sf::Vector2f & offset) const;
			Ray operator-(const sf::Vector2f & offset) const;
			Ray& operator+=(const sf::Vector2f& offset);
			Ray& operator-=(const sf::Vector2f& offset);
			Ray operator*(float factor) const;
			Ray operator/(float divisor) const;
			Ray& operator*=(float factor);
			Ray& operator/=(float divisor);

			/*
			 * \return a normalized function. length of the direction vector is 1 
			 */
			Ray getNormalized() const;

			/*
			 * \brief Will set the length of the direction vector to 1. The angle will not change
			 */
			void normalize();

			void setPosition(const sf::Vector2f& pos);
			void setPosition(float posX, float posY);
			const sf::Vector2f& getPosition() const;

			void setDirection(const sf::Vector2f& dir);
			void setDirection(float dirX, float dirY);
			const sf::Vector2f& getDirection() const;

			float getAngleRAD() const;

			/*
				Returns the clockwise angle in radiant between vec1 and vec2.
				The angle points from vec1 to vec2. If the direction of that angle is clockwise, the angle is negative.
			*/
			float getAngleRAD(const Ray& ray) const;

			sf::Vector2f getPoint(float scalar) const;

			/**
			 * \brief Calculates the <scalar> "s" in this given function F(s) = pos + s * dir, 
			 *	      for which the distance |F(s) - point| is a minimum.
			 *
			 * \param point which from the shortest distanced point on this function is searched.
			 * \param failed will be set to true, if the direction vector of this function has a length of 0.
			 * \return A scalar which is used to find the closest point on this function. 
			 *		   Call getPoint(getShortestDistanceFactor(...)) to get the shortest distanced point.
			 * 
			 */
			float getShortestDistanceFactor(const sf::Vector2f& point, bool *failed = nullptr) const;

			/**
			 * \brief Calculates the shortest distance from the <point> to this function
			 *
			 * \param point which from the shortest distanced is searched from.
			 * \param failed will be set to true, if the direction vector of this function has a length of 0.
			 * \return The shortest distance from this function to the <point>
			 *
			 */
			float getShortestDistance(const sf::Vector2f& point, bool* failed = nullptr) const;


			/**
			 * \brief Calculates the collision between two linear vector functions
			 * For the given Functions:
			 *      Fa(a) := pointA + a * directionA
			 *      Fb(b) := pointB + b * directionB
			 *
			 *      solve(Fa(a) = Fb(b), solveFor {a,b})
			 *
			 * \param pointA is the offset A for function A.
			 * \param directionA is the direction vector, which gets added at the end of pointA
			 * \param pointB is the offset B for function B.
			 * \param directionB is the direction vector, which gets added at the end of pointB
			 * \param outDistanceFactorA is the calculated scalar a
			 * \param outDistanceFactorB is the calculated scalar b
			 * \return false, if no collision is possible. (crossP(directionA, directionB) = 0 ==> directionA is parallel to directionB)
			 *                outDistanceFactorA and outDistanceFactorB will be set to NAN
			 *         true,  if collision is possible
			 *                outDistanceFactorA will be the factor for Fa(outDistanceFactorA)
			 *                outDistanceFactorB will be the factor for Fb(outDistanceFactorB)
			 */
			static bool raycast(const sf::Vector2f& pointA,
							    const sf::Vector2f& directionA,
							    const sf::Vector2f& pointB,
							    const sf::Vector2f& directionB,
							    float& outDistanceFactorA,
							    float& outDistanceFactorB);
			
			/**
			 * \brief Calculates the collision between two linear vector functions
			 * For the given Functions:
			 *      Fa(a) := pointA + a * directionA
			 *      Fb(b) := pointB + b * directionB
			 *
			 *      solve(Fa(a) = Fb(b), solveFor {a,b})
			 *
			 * \param other linear vector function which is used as Fb(b)
			 * \param outDistanceFactorA is the calculated scalar a
			 * \param outDistanceFactorB is the calculated scalar b
			 * \return false, if no collision is possible. (crossP(directionA, directionB) = 0 ==> directionA is parallel to directionB)
			 *                outDistanceFactorA and outDistanceFactorB will be set to NAN
			 *         true,  if collision is possible
			 *                outDistanceFactorA will be the factor for Fa(outDistanceFactorA)
			 *                outDistanceFactorB will be the factor for Fb(outDistanceFactorB)
			 */
			bool raycast(const Ray& other,
						 float& outDistanceFactorA,
						 float& outDistanceFactorB) const;

			/**
			 * \brief Calculates the collision between this vector function and a circle
			 *
			 * \param center of the circle
			 * \param radius of the circle
			 * \param output of the first collision point
			 * \param output of the second collision point if it exists
			 * \return the amount of collision points, 0 if no collision occured
			 */
			int getCircleCollisionFactors(const sf::Vector2f& circlePos, float circleRadius,
										  float& outFactor1, float& outFactor2) const;

			/**
			 * \brief Checks if the given shape collides with this ray
			 * 
			 * \param shape which is checked for collision
			 * \param outDistanceFactor is the calculated scalar for the ray
			 * \param outEdge is the index of shapeVertex where the collision occured between points[outEdge] -- points[outEdge+1].
			 * \return true, if the shape collides with the ray. The scalar for the ray is stored in outDistanceFactor
			 *         false, if the shape does not collide with the ray. outDistanceFactor is not changed.
			 */ 
			bool raycast(const Components::Shape& shape, float &outDistanceFactor, size_t &outEdge) const;
			bool raycast(const QSFML::vector<Components::Shape>& shapes, float &outDistanceFactor, size_t &outShapeIndex, size_t &outEdge) const;
			bool raycast(const QSFML::vector<Components::Shape*>& shapes, float &outDistanceFactor, size_t &outShapeIndex, size_t &outEdge) const;
			
			bool raycast(const AABB& aabb, float &outDistanceFactor, size_t &outEdge) const;
		
			void drawGizmos(sf::RenderTarget& target, sf::RenderStates states) const;


			private:
			bool raycast_internal(const Components::Shape& shape, float& outDistanceFactor, size_t& outEdge) const;
			bool raycast_internal(const AABB& aabb, float& outDistanceFactor, size_t& outEdge) const;


			sf::Vector2f m_pos;
			sf::Vector2f m_dir;
			float m_dirLength;
			mutable QSFML::vector<sf::Vector2f> m_gizmoPoints;
			mutable QSFML::vector<QSFML::pair<sf::Vector2f, sf::Vector2f>> m_gizmoLines;


			static sf::Color s_gizmoLineColor;
			static sf::Color s_gizmoPointColor;
			static float s_gizmoPointRadius;
			static size_t s_maxGizmoCount;
		};
	}
}
