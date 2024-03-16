#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class PerlinNoise
		{
		public:
			PerlinNoise();
			PerlinNoise(unsigned int seed);
			~PerlinNoise();

			float noise(float x, float y, int octaves, const sf::Vector2u& gridsize) const;

		private:
			sf::Vector2f randomGradient(int ix, int iy) const;
			float dotGridGradient(int ix, int iy, float x, float y) const;

			float interpolate(float a0, float a1, float w) const;
			float perlin(float x, float y) const;

			unsigned int m_seed;
		};
	}
}
