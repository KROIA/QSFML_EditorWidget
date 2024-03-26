#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT RandomEngine
		{
			public:
				static void init();

				static float getFloat(float min = 0, float max = 1);
				static sf::Vector2f getVector(const sf::Vector2f& minRange = sf::Vector2f(0, 0), const sf::Vector2f& maxRange = sf::Vector2f(1, 1));

				static void fillArray(float* array, unsigned int size, float min = 0, float max = 1);

				static void setSeed(unsigned int seed);
				static unsigned int getSeed();

				
			private:
				RandomEngine();
				static RandomEngine &getInstance();

				unsigned int m_seed;
		};
	}
}
