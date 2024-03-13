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

			float noise(float x, float y) const;

		private:
			void setup(unsigned int seed);
			void bakeGrid();
			
			inline float fade(float t) const
			{
				return t * t * t * (t * (t * 6 - 15) + 10);
			}
			inline float lerp(float t, float a, float b) const
			{
				return a + t * (b - a);
			}
			inline float grad(int hash, float x, float y) const
			{
				int h = hash & 15;
				float u = h < 8 ? x : y;
				float v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
				return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
			}

			//unsigned int m_permutation[512];
			float* m_randTable;
			float* m_noiseGrid;
			unsigned int m_tableSize;
			unsigned int m_seed;
		};
	}
}
