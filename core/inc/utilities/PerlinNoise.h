#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_API PerlinNoise
		{
		public:
			PerlinNoise();
			PerlinNoise(unsigned int seed);
			~PerlinNoise();

			double noise(double x, double y, int octaves, const sf::Vector2u& gridsize) const;

		private:
			sf::Vector2f randomGradient(int ix, int iy) const;
			double dotGridGradient(int ix, int iy, double x, double y) const;

			double interpolate(double a0, double a1, double w) const;
			double perlin(double x, double y) const;

			unsigned int m_seed;
		};
	}
}
