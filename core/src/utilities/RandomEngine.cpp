#include "utilities/RandomEngine.h"

#include <cstdlib>
#include <ctime>

namespace QSFML
{
	namespace Utilities
	{
		RandomEngine RandomEngine::s_instance;


		RandomEngine::RandomEngine()
		{
			// Seed the random number generator with the current time
			setSeed(static_cast<unsigned int>(std::time(nullptr)));
		}
		void RandomEngine::setSeed(unsigned int seed)
		{
			s_instance.m_seed = seed;
			std::srand(seed);
		}
		unsigned int RandomEngine::getSeed()
		{
			return s_instance.m_seed;
		}
		float RandomEngine::getFloat(float min, float max)
		{
			// Generate a random number within the given range
			float randomNumber = min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));

			return randomNumber;
		}

		sf::Vector2f RandomEngine::getVector(const sf::Vector2f& minRange, const sf::Vector2f& maxRange)
		{
			sf::Vector2f randVec(Utilities::RandomEngine::getFloat(minRange.x, maxRange.x),
								 Utilities::RandomEngine::getFloat(minRange.y, maxRange.y));
			return randVec;
		}

	}
}
