#include "utilities/PerlinNoise.h"
#include <cmath>

namespace QSFML
{
	namespace Utilities
	{

		PerlinNoise::PerlinNoise()
			: m_randTable(nullptr)
			, m_noiseGrid(nullptr)
		{
			setup(0);
		}
		PerlinNoise::PerlinNoise(unsigned int seed)
			: m_randTable(nullptr)
			, m_noiseGrid(nullptr)
		{
			setup(seed);
		}
		PerlinNoise::~PerlinNoise()
		{
			delete[] m_randTable;
			delete[] m_noiseGrid;
		}
		float PerlinNoise::noise(float x, float y) const
		{
			x *= m_tableSize;
			y *= m_tableSize;
			unsigned int X = (unsigned int)floor(x) & (m_tableSize - 1);
			unsigned int Y = (unsigned int)floor(y) & (m_tableSize - 1);

			float dx = x - floor(x);
			float dy = y - floor(y);

			float u = fade(dx);
			float v = fade(dy);

			float noiseValue = lerp(u, m_noiseGrid[(Y* m_tableSize)+X], 
									   m_noiseGrid[(Y * m_tableSize)+(X + 1) % m_tableSize]);
			noiseValue += lerp(v, m_noiseGrid[(Y * m_tableSize) + X], 
				                  m_noiseGrid[((Y + 1) % m_tableSize) * m_tableSize + X]);

			return noiseValue;

			/*
			int X = (int)floor(x) & 255;
			int Y = (int)floor(y) & 255;

			x -= floor(x);
			y -= floor(y);

			float u = fade(x);
			float v = fade(y);

			int AA = m_permutation[m_permutation[X] + Y];
			int AB = m_permutation[m_permutation[X] + Y + 1];
			int BA = m_permutation[m_permutation[X + 1] + Y];
			int BB = m_permutation[m_permutation[X + 1] + Y + 1];

			return lerp(v, lerp(u, grad(AA, x, y), grad(BA, x - 1, y)), lerp(u, grad(AB, x, y - 1), grad(BB, x - 1, y - 1)));
			*/
		}

		void PerlinNoise::setup(unsigned int seed)
		{
			m_seed = seed;
			m_tableSize = 256;
			srand(seed);

			m_randTable = new float[m_tableSize * m_tableSize];
			m_noiseGrid = new float[m_tableSize * m_tableSize];
			for (int i = 0; i < m_tableSize * m_tableSize; i++)
			{
				m_randTable[i] = static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX));
			}
			
			bakeGrid();
			/*// Initialize m_permutation table
			for (int i = 0; i < 256; i++) {
				m_permutation[i] = i;
			}

			// Shuffle the m_permutation table
			for (int i = 0; i < 256; i++) {
				int j = rand() % 256;
				std::swap(m_permutation[i], m_permutation[j]);
			}

			// Duplicate the m_permutation table to avoid overflow
			for (int i = 0; i < 256; i++) {
				m_permutation[i + 256] = m_permutation[i];
			}*/
		}
		void PerlinNoise::bakeGrid()
		{
			memcpy(m_noiseGrid, m_randTable, m_tableSize * m_tableSize * sizeof(float));
			/*for (size_t i = 0; i < 8; ++i)
			{
				for (size_t x = 0; x < m_tableSize; ++x)
				{
					for (size_t y = 0; y < m_tableSize; ++y)
					{
						unsigned int pitch = 1 << i;
						if (x % pitch == 0 && y % pitch == 0)
						{


						}
						
						
						float value = m_noiseGrid[x * m_tableSize + y];
						float value2 = m_noiseGrid[(x + 1) * m_tableSize + y];
						float value3 = m_noiseGrid[x * m_tableSize + (y + 1)];
						float value4 = m_noiseGrid[(x + 1) * m_tableSize + (y + 1)];
						float value5 = (value + value2 + value3 + value4) / 4.0f;
						m_noiseGrid[x * m_tableSize + y] = value5;
					}
				}
			}*/
		}

		
	}
}