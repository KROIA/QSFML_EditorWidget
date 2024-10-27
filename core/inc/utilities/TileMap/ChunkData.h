#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics/Color.hpp>


namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT ChunkData
		{
			friend class Chunk;
			public:
			static constexpr int CHUNK_SIZE = 16;

			ChunkData();
			virtual ~ChunkData();


			const sf::Vector2i& getPosition() const { return m_position; }

			void setTextureIndex(size_t x, size_t y, size_t idx)
			{
				m_textureIdx[y * CHUNK_SIZE + x] = idx;
			}
			const std::vector<size_t>& getTextureIndex() const 
			{
				return m_textureIdx; 
			}
			size_t getTextureIndex(size_t x, size_t y) const
			{
				return m_textureIdx[y * CHUNK_SIZE + x];
			}

			const sf::Color& getLowLevelReplacementColor() const
			{
				if (!m_lowLevelReplacementColorUpdated)
				{
					m_lowLevelReplacementColor = createLowLevelReplacementColor();
					m_lowLevelReplacementColorUpdated = true;
				}
				return m_lowLevelReplacementColor;
			}

			void setLowLevelReplacementColor(const sf::Color& color)
			{
				m_lowLevelReplacementColor = color;
				m_lowLevelReplacementColorUpdated = true;
			}

			protected:
			virtual sf::Color createLowLevelReplacementColor() const;

			private:
			void setPosition(const sf::Vector2i& pos)
			{
				m_position = pos;
			}

			sf::Vector2i m_position;
			std::vector<size_t> m_textureIdx;

			mutable sf::Color m_lowLevelReplacementColor;
			mutable bool m_lowLevelReplacementColorUpdated;
		};

	}
}