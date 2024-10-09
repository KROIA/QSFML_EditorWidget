#include "utilities/RandomEngine.h"

#include <cstdlib>
#include <ctime>

namespace QSFML
{
	namespace Utilities
	{
		RandomEngine::RandomEngine()
			: m_seed(static_cast<unsigned int>(std::time(nullptr)))
		{
			// Seed the random number generator with the current time
			std::srand(m_seed);
		}
		RandomEngine& RandomEngine::getInstance()
		{
			static RandomEngine instance;
			return instance;
		}
	    void RandomEngine::init()
		{
			getInstance();
		}
		void RandomEngine::setSeed(unsigned int seed)
		{
			getInstance().m_seed = seed;
			std::srand(seed);
		}
		unsigned int RandomEngine::getSeed()
		{
			return getInstance().m_seed;
		}
		float RandomEngine::getFloat(float min, float max)
		{
			// Generate a random number within the given range
			float randomNumber = min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
			return randomNumber;
		}
		int RandomEngine::getInt(int min, int max)
		{
			// Generate a random number within the given range
			int randomNumber = min + std::rand() % (max - min + 1);
			return randomNumber;
		}

		sf::Vector2f RandomEngine::getVector(const sf::Vector2f& minRange, const sf::Vector2f& maxRange)
		{
			sf::Vector2f randVec(getFloat(minRange.x, maxRange.x),
								 getFloat(minRange.y, maxRange.y));
			return randVec;
		}
		sf::Color RandomEngine::getColor()
		{
			return sf::Color(getInt(0, 255), getInt(0, 255), getInt(0, 255));
		}
		sf::Color RandomEngine::getColor(const sf::Color& minRange, const sf::Color& maxRange)
		{
			sf::Color randColor(getInt(minRange.r, maxRange.r),
								getInt(minRange.g, maxRange.g),
								getInt(minRange.b, maxRange.b),
								getInt(minRange.a, maxRange.a));
			return randColor;
		}
		void RandomEngine::fillArray(float* array, unsigned int size, float min, float max)
		{
			for (unsigned int i = 0; i < size; ++i)
			{
				array[i] = getFloat(min, max);
			}
		}

	}
}
