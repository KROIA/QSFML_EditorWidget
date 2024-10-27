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
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_3);
			QSFMLP_GENERAL_BLOCK("m_textureIdx.clear()", QSFML_COLOR_STAGE_4);
			m_textureIdx.clear();
			QSFMLP_GENERAL_END_BLOCK;
		}



		sf::Color ChunkData::createLowLevelReplacementColor() const
		{
			return sf::Color::Transparent;
		}

	}
}