#pragma once

#include "QSFML_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT RandomEngine
		{
				RandomEngine();
			public:
			

				static float getFloat(float min = 0, float max = 1);
				static sf::Vector2f getVector(const sf::Vector2f& minRange = sf::Vector2f(0, 0), const sf::Vector2f& maxRange = sf::Vector2f(1, 1));

				static void setSeed(unsigned int seed);
				static unsigned int getSeed();
			private:
				static RandomEngine s_instance;
				unsigned int m_seed;
		};
	}
}