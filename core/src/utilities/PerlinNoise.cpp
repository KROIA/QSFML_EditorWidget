#include "utilities/PerlinNoise.h"
#include "utilities/VectorOperations.h"
#include <cmath>
#include <algorithm>
#include <random>

namespace QSFML
{
	namespace Utilities
	{
		PerlinNoise::PerlinNoise() 
		{
			m_seed = 0;
		}

		PerlinNoise::PerlinNoise(unsigned int seed) 
		{
			m_seed = seed;
		}

		PerlinNoise::~PerlinNoise() 
		{}

		double PerlinNoise::noise(double x, double y, int octaves, const sf::Vector2u& gridsize) const
		{
			//QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			double value = 0.0f;
			double amplitude = 1.0f;
			double frequency = 1.0f;
			double maxValue = 0.0f;  // Used for normalizing result to 0.0 - 1.0

			double gridSize_x = 1.f/gridsize.x;
			double gridSize_y = 1.f/gridsize.y;

			for (int i = 0; i < octaves; i++) {
				value += perlin(x * (frequency * gridSize_x), y * (frequency * gridSize_y)) * amplitude;
				maxValue += amplitude;
				amplitude *= 0.5f;
				frequency *= 2.0f;
			}
			value = value * 2 / maxValue;
			/*if (value < -1)
				value = -1;
			else if (value > 1)
				value = 1;*/
			return value;
		}

		sf::Vector2f PerlinNoise::randomGradient(int ix, int iy) const 
		{
			// No precomputed gradients mean this works for any number of grid coordinates
			const unsigned w = 8 * sizeof(unsigned);
			const unsigned s = w / 2;
			unsigned a = ix, b = iy;
			a *= 3284157443;
			a ^= m_seed; // Incorporate the seed into the hash calculation

			b ^= a << s | a >> w - s;
			b *= 1911520717;
			b ^= m_seed; // Incorporate the seed into the hash calculation

			a ^= b << s | b >> w - s;
			a *= 2048419325;
			a ^= m_seed; // Incorporate the seed into the hash calculation

			double random = (double)a * ((double)M_PI / (double)(~(~0u >> 1))); // in [0, 2*Pi]

			// Create the vector from the angle
			sf::Vector2f v;
			v.x = sin(random);
			v.y = cos(random);

			return v;
		}

		// Computes the dot product of the distance and gradient vectors.
		double PerlinNoise::dotGridGradient(int ix, int iy, double x, double y) const 
		{
			// Get gradient from integer coordinates
			sf::Vector2f gradient = randomGradient(ix, iy);

			// Compute the distance vector
			double dx = x - (double)ix;
			double dy = y - (double)iy;

			// Compute the dot-product
			return (dx * gradient.x + dy * gradient.y);
		}

		double PerlinNoise::interpolate(double a0, double a1, double w) const
		{
			return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
		}


		// Sample Perlin noise at coordinates x, y
		double PerlinNoise::perlin(double x, double y) const
		{
			//QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_2);
			// Determine grid cell corner coordinates
			int x0 = (int)x;
			int y0 = (int)y;
			int x1 = x0 + 1;
			int y1 = y0 + 1;

			// Compute Interpolation weights
			double sx = x - (double)x0;
			double sy = y - (double)y0;

			// Compute and interpolate top two corners
			double n0 = dotGridGradient(x0, y0, x, y);
			double n1 = dotGridGradient(x1, y0, x, y);
			double ix0 = interpolate(n0, n1, sx);

			// Compute and interpolate bottom two corners
			n0 = dotGridGradient(x0, y1, x, y);
			n1 = dotGridGradient(x1, y1, x, y);
			double ix1 = interpolate(n0, n1, sx);

			// Final step: interpolate between the two previously interpolated values, now in y
			double value = interpolate(ix0, ix1, sy);


			return value;
		}
		


		
	}
}
