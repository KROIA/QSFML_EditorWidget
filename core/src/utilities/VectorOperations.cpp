#include "utilities/VectorOperations.h"
#include "utilities/RandomEngine.h"
#include <SFML/Graphics.hpp>

#define M_2PI (2 * M_PI)

namespace QSFML
{
namespace VectorMath
{
    float getLength(const sf::Vector2f &vec)
    {
        return (float)sqrt((double)(vec.x * vec.x + vec.y * vec.y));
    }
    double getLength(const Vector2d &vec)
    {
        return (float)sqrt((vec.x * vec.x + vec.y * vec.y));
    }
    float getSquareLength(const sf::Vector2f &vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }
    double getSquareLength(const Vector2d& vec)
    {
        return vec.x * vec.x + vec.y * vec.y;
    }
    sf::Vector2f getRotatedUnitVectorRAD(float rad)
    {
        return sf::Vector2f((float)cos((double)rad), (float)sin((double)rad));
    }
    Vector2d getRotatedUnitVectorRAD(double rad)
    {
        return Vector2d(cos(rad), sin(rad));
    }
    sf::Vector2f getRotatedUnitVector(float deg)
    {
		float rad = deg * M_PI / 180;
        return sf::Vector2f((float)cos((double)rad), (float)sin((double)rad));
    }
    Vector2d getRotatedUnitVector(double deg)
    {
        double rad = deg * M_PI / 180;
        return Vector2d(cos(rad), sin(rad));
    }
    sf::Vector2f getRotatedRAD(const sf::Vector2f &vec, float rad)
    {
        float angle = getAngleRAD(vec);
        angle += rad;
        return sf::Vector2f((float)cos((double)angle), (float)sin((double)angle)) * getLength(vec);
    }
    Vector2d getRotatedRAD(const Vector2d& vec, double rad)
    {
        double angle = getAngleRAD(vec);
        angle += rad;
        return Vector2d(cos(angle), sin(angle)) * getLength(vec);
    }
    sf::Vector2f getRotated(const sf::Vector2f& vec, float deg)
    {
        float angle = getAngleRAD(vec);
        float rad = deg * M_PI / 180;
        angle += rad;
        return sf::Vector2f((float)cos((double)angle), (float)sin((double)angle)) * getLength(vec);
    }
    Vector2d getRotated(const Vector2d& vec, double deg)
    {
        double angle = getAngleRAD(vec);
        double rad = deg * M_PI / 180;
        angle += rad;
        return Vector2d(cos(angle), sin(angle)) * getLength(vec);
    }
    sf::Vector2f getRotatedRAD(const sf::Vector2f &vec, const sf::Vector2f &origin, float rad)
    {
        return getRotatedRAD(vec - origin, rad) + origin;
    }
    Vector2d getRotatedRAD(const Vector2d& vec, const Vector2d& origin, double rad)
    {
        return getRotatedRAD(vec - origin, rad) + origin;
    }
    sf::Vector2f getRotated(const sf::Vector2f& vec, const sf::Vector2f& origin, float deg)
    {
        return getRotated(vec - origin, deg) + origin;
    }
    Vector2d getRotated(const Vector2d& vec, const Vector2d& origin, double deg)
    {
        return getRotated(vec - origin, deg) + origin;
    }
    float getAngleRAD(const sf::Vector2f& vec)
    {
#ifdef QSFML_OPTIMIZED
        float angle = atan2(vec.y, vec.x);
        angle = fmod(angle + M_PI, 2 * M_PI);
        return (angle < 0) ? angle + 2 * M_PI - M_PI : angle - M_PI;

#else

        float sqrL = getSquareLength(vec);
        if(sqrL <= 0)
            return 0;
        float angle;
        sqrL = (float)sqrt((double)sqrL);

        if (vec.y <= 0) {
            angle = -(float)acos((double)(vec.x / sqrL));
        }
        else {
            angle = (float)acos((double)(vec.x / sqrL));
        }
        return getNormalzedAngleRAD(angle);
#endif
        return 0;
    }
    double getAngleRAD(const Vector2d& vec)
    {
#ifdef QSFML_OPTIMIZED
        double sqrL = vec.x * vec.x + vec.y * vec.y;

        if (sqrL == 0)
            return 0;

        double angle = atan2(vec.y, vec.x);
        angle = fmod(angle + M_PI, 2 * M_PI);

        return (angle < 0) ? angle + 2 * M_PI - M_PI : angle - M_PI;
#else

        double sqrL = getSquareLength(vec);
        if (sqrL <= 0)
            return 0;
        double angle;
        sqrL = sqrt(sqrL);

        if (vec.y <= 0) {
            angle = -acos(vec.x / sqrL);
        }
        else {
            angle = acos(vec.x / sqrL);
        }
        return getNormalzedAngleRAD(angle);
#endif
        return 0;
    }

    float getAngle(const sf::Vector2f& vec)
    {
		return getAngleRAD(vec) * 180 / M_PI;
    }
    double getAngle(const Vector2d& vec)
    {
		return getAngleRAD(vec) * 180 / M_PI;
    }



    float getAngleRAD(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        /*float angle1 = atan2(vec1.y, vec1.x);
        float angle2 = atan2(vec2.y, vec2.x);

        float angle = angle2 - angle1;
        angle = fmod(angle + M_PI, 2 * M_PI);
        return (angle < 0) ? angle + M_PI : angle - M_PI;*/

        // Calculate the magnitudes of the vectors
        float lengthProduct = (float)sqrt((double)((vec1.x * vec1.x + vec1.y * vec1.y) * (vec2.x * vec2.x + vec2.y * vec2.y)));

        if(lengthProduct == 0)
			return 0;

        // Calculate the dot product
        float dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;

        // Calculate the cosine of the angle between the vectors
        float cosAngle = dotProduct / lengthProduct;
        if (cosAngle > 1.0)
            cosAngle = 1.0;
        else if (cosAngle < -1.0)
            cosAngle = -1.0;

        // Calculate the angle in radians using the arc cosine function (acos)
        float angle = (float)acos((double)cosAngle);

        // Determine the sign of the angle using the cross product
        float crossProduct = vec1.x * vec2.y - vec1.y * vec2.x;
        if (crossProduct < 0) {
            return -angle; // Adjust angle if it's negative
        }
        return angle;

        
    }
    
    double getAngleRAD(const Vector2d& vec1, const Vector2d& vec2)
    {
        // Calculate the magnitudes of the vectors
        double lengthProduct = sqrt((vec1.x * vec1.x + vec1.y * vec1.y) * (vec2.x * vec2.x + vec2.y * vec2.y));

        if (lengthProduct == 0)
			return 0;

        // Calculate the dot product
        double dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;

        // Calculate the cosine of the angle between the vectors
        double cosAngle = dotProduct / lengthProduct;
        if(cosAngle > 1.0)
			cosAngle = 1.0;
		else if(cosAngle < -1.0)
			cosAngle = -1.0;

        // Calculate the angle in radians using the arc cosine function (acos)
        double angle = acos(cosAngle);

        // Determine the sign of the angle using the cross product
        double crossProduct = vec1.x * vec2.y - vec1.y * vec2.x;
        if (crossProduct < 0) {
            return -angle; // Adjust angle if it's negative
        }
        return angle;
        /*
        double dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;
        double lengthProduct = sqrt((vec1.x * vec1.x + vec1.y * vec1.y) * (vec2.x * vec2.x + vec2.y * vec2.y));
        double angle = acos(dotProduct / lengthProduct);
        return getNormalzedAngleRAD(angle);
        */
        
        /*double dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;
        double determinant = vec1.x * vec2.y - vec1.y * vec2.x;
        double length = getLength(vec1) * getLength(vec2);

        double angle = 0.0f;

        if (determinant >= 0) {
            angle = acos(dotProduct / length);
        }
        else {
            angle = 2 * M_PI - acos(dotProduct / length);
        }

        return getNormalzedAngleRAD(angle);*/
    }
    float getAngle(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
    {
		return getAngleRAD(vec1, vec2) * 180 / M_PI;
    }
    double getAngle(const Vector2d& vec1, const Vector2d& vec2)
    {
		return getAngleRAD(vec1, vec2) * 180 / M_PI;
    }
    float dotProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }
    double dotProduct(const Vector2d& vec1, const Vector2d& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }
    float crossProduct(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
    {
        return vec1.x * vec2.y - vec1.y * vec2.x;
    }
    double crossProduct(const Vector2d& vec1, const Vector2d& vec2)
    {
        return vec1.x * vec2.y - vec1.y * vec2.x;
    }
    const sf::Vector2f &getUnitVector()
    {
        static const sf::Vector2f v(1,0);
        return v;
    }
    const Vector2d& getUnitVectorD()
    {
        static const Vector2d v(1, 0);
        return v;
    }
    sf::Vector2f getNormalized(const sf::Vector2f &vec)
    {
        float l = vec.x * vec.x + vec.y * vec.y;
        if(l <= 0)
            return vec;
        return(vec / (float)sqrt((double)l));
    }
    Vector2d getNormalized(const Vector2d& vec)
    {
        double l = vec.x * vec.x + vec.y * vec.y;
        if (l <= 0)
            return vec;
        return(vec / sqrt(l));
    }

    float getNormalzedAngleRAD(float rad)
    {
#ifdef QSFML_OPTIMIZED
        rad = fmod(rad + M_PI, 2 * M_PI);
        return (rad < 0) ? rad + M_PI : rad - M_PI;
#else
        rad = (float)fmod((double)rad + M_PI, 2 * M_PI);
        if (rad < 0)
            rad += 2 * M_PI;
        return rad - (float)M_PI;
#endif
    }
    double getNormalzedAngleRAD(double rad)
    {
#ifdef QSFML_OPTIMIZED
        rad = fmod(rad + M_PI, 2 * M_PI);
        return (rad < 0) ? rad + M_PI : rad - M_PI;
#else
        rad = fmod(rad + M_PI, 2 * M_PI);
        if (rad < 0)
            rad += 2 * M_PI;
        return rad - M_PI;
#endif
    }
    float getNormalzedAngle(float deg)
    {
		deg = fmod(deg, 360);
		if (deg < 0)
			deg += 360;
		return deg - 180;
    }
    double getNormalzedAngle(double deg)
    {
		deg = fmod(deg, 360);
		if (deg < 0)
			deg += 360;
		return deg - 180;
    }
    bool isAngleInRangeRAD(float rad, float minRad, float maxRad)
    {
        // Normalize angles to be in the range [0, 2*pi)
        rad = (float)fmod((double)rad, M_2PI);
        if (rad < 0)
            rad += M_2PI;

        minRad = (float)fmod((double)minRad, M_2PI);
        if (minRad < 0)
            minRad += M_2PI;

        maxRad = (float)fmod((double)maxRad, M_2PI);
        if (maxRad < 0)
            maxRad += M_2PI;

        // Case where the rad wraps around
        if (minRad > maxRad) {
            return (rad >= minRad || rad <= maxRad);
        }

        // Case where the rad does not wrap around
        return (rad >= minRad && rad <= maxRad);
    }
    bool isAngleInRangeRAD(double rad, double minRad, double maxRad)
    {
        // Normalize angles to be in the range [0, 2*pi)
        rad = fmod(rad, M_2PI);
        if (rad < 0)
            rad += M_2PI;

        minRad = fmod(minRad, M_2PI);
        if (minRad < 0)
            minRad += M_2PI;

        maxRad = fmod(maxRad, M_2PI);
        if (maxRad < 0)
            maxRad += M_2PI;

        // Case where the rad wraps around
        if (minRad > maxRad) {
            return (rad >= minRad || rad <= maxRad);
        }

        // Case where the rad does not wrap around
        return (rad >= minRad && rad <= maxRad);
    }
    bool isAngleInRange(float deg, float minDeg, float maxDeg)
    {
        // Normalize angles to be in the range [0, 360)
        deg = (float)fmod((double)deg, 360);
        if (deg < 0)
            deg += 360;

        minDeg = (float)fmod((double)minDeg, 360);
        if (minDeg < 0)
            minDeg += 360;

        maxDeg = (float)fmod((double)maxDeg, 360);
        if (maxDeg < 0)
            maxDeg += 360;

        // Case where the rad wraps around
        if (minDeg > maxDeg) {
            return (deg >= minDeg || deg <= maxDeg);
        }

        // Case where the rad does not wrap around
        return (deg >= minDeg && deg <= maxDeg);
    }
    bool isAngleInRange(double deg, double minDeg, double maxDeg)
    {
        // Normalize angles to be in the range [0, 360)
        deg = fmod(deg, 360);
        if (deg < 0)
            deg += 360;

        minDeg = fmod(minDeg, 360);
        if (minDeg < 0)
            minDeg += 360;

        maxDeg = fmod(maxDeg, 360);
        if (maxDeg < 0)
            maxDeg += 360;

        // Case where the rad wraps around
        if (minDeg > maxDeg) {
            return (deg >= minDeg || deg <= maxDeg);
        }

        // Case where the rad does not wrap around
        return (deg >= minDeg && deg <= maxDeg);
    }
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
	{
		return start + (end - start) * t;
	}
	Vector2d lerp(const Vector2d& start, const Vector2d& end, double t)
	{
		return start + (end - start) * t;
	}
    sf::Vector2f lerp(const QSFML::vector<sf::Vector2f>& path, float t)
    {
        if (path.size() == 0)
            return sf::Vector2f(0, 0);
        if (path.size() == 1)
            return path[0];
        if (t <= 0)
            return path[0];
        if (t >= 1)
            return path[path.size() - 1];

        double pathLength = 0;
        for (size_t i = 1; i < path.size(); i++)
        {
            pathLength += getLength(path[i] - path[i - 1]);
        }

        double currentLength = 0;
        for (size_t i = 1; i < path.size(); i++)
        {
            double length = getLength(path[i] - path[i - 1]);
            if (currentLength + length >= pathLength * t)
            {
                double t2 = (pathLength * t - currentLength) / length;
                return lerp(path[i - 1], path[i], t2);
            }
            currentLength += length;
        }
        return path[path.size() - 1];
    }

    Vector2d lerp(const QSFML::vector<Vector2d>& path, double t)
    {
        if (path.size() == 0)
            return Vector2d(0, 0);
        if (path.size() == 1)
            return path[0];
        if (t <= 0)
            return path[0];
        if (t >= 1)
            return path[path.size() - 1];

        double pathLength = 0;
        for (size_t i = 1; i < path.size(); i++)
        {
            pathLength += getLength(path[i] - path[i - 1]);
        }

        double currentLength = 0;
        for (size_t i = 1; i < path.size(); i++)
        {
            double length = getLength(path[i] - path[i - 1]);
            if (currentLength + length >= pathLength * t)
            {
                double t2 = (pathLength * t - currentLength) / length;
                return lerp(path[i - 1], path[i], t2);
            }
            currentLength += length;
        }
        return path[path.size() - 1];
    }

    sf::Vector2f getScale(const sf::Transform& transform)
    {
        // Extract the matrix values from sf::Transform
        const float* matrix = transform.getMatrix();

        // The scaling components are derived from the matrix values
        float scaleX = std::sqrt(matrix[0] * matrix[0] + matrix[1] * matrix[1]);
        float scaleY = std::sqrt(matrix[4] * matrix[4] + matrix[5] * matrix[5]);

        return sf::Vector2f(scaleX, scaleY);
    }

    float getRotation(const sf::Transform& transform)
	{
		// Extract the matrix values from sf::Transform
		const float* matrix = transform.getMatrix();

		// The rotation components are derived from the matrix values
		return std::atan2(matrix[1], matrix[0]) * 180 / M_PI;
	}
}
}
