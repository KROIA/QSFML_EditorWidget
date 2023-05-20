#pragma once
#include "QSFML_global.h"
#include <SFML/System/Vector2.hpp>


namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT Ray
		{
		public:
			Ray();
			Ray(const Ray& other);
			Ray(const sf::Vector2f& position, const sf::Vector2f& direction);
			Ray(float posX, float posY, float dirX, float dirY);

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

			void setPos(const sf::Vector2f& pos);
			void setPos(float posX, float posY);
			const sf::Vector2f& getPos() const;

			void setDir(const sf::Vector2f& dir);
			void setDir(float dirX, float dirY);
			const sf::Vector2f& getDir() const;

			sf::Vector2f getPoint(float scalar);

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
			float getShortestDistanceFactor(const sf::Vector2f& point, bool *failed = nullptr);

			/**
			 * \brief Calculates the shortest distance from the <point> to this function
			 *
			 * \param point which from the shortest distanced is searched from.
			 * \param failed will be set to true, if the direction vector of this function has a length of 0.
			 * \return The shortest distance from this function to the <point>
			 *
			 */
			float getShortestDistance(const sf::Vector2f& point, bool* failed = nullptr);


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
		private:
			sf::Vector2f m_pos;
			sf::Vector2f m_dir;
			float m_dirLength;
		};
	}
}