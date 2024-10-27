#include "utilities/TileMap/ChunkData.h"

namespace QSFML
{
	namespace Utilities
	{
		ChunkData::ChunkData()
			: m_textureIdx(CHUNK_SIZE * CHUNK_SIZE)
		{

		}
		ChunkData::~ChunkData()
		{

		}



		sf::Color ChunkData::createLowLevelReplacementColor() const
		{
			return sf::Color::Transparent;
		}

	}
}