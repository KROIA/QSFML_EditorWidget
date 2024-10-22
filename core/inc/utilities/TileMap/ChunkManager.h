#pragma once

#include "QSFML_EditorWidget_base.h"
#include "Chunk.h"
#include "assets/TextureMap.h"


namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT ChunkManager
		{
			struct VectorHash {
				std::size_t operator()(const sf::Vector2i& v) const {
					size_t hashed = 0;

					static constexpr int shiftCount = sizeof(int) * 8;
					long xMasked = ((long)v.x << shiftCount);
					long yMasked = ((long)v.y & (~long(0) >> shiftCount));
					// shorten the int range and combine both coordinates to one value
					hashed = xMasked | yMasked;
					return hashed;
				}
			};
			public:
			static Log::LogObject& getLogger();

			ChunkManager(const sf::Texture& texture, const sf::Vector2u& mapDim);
			~ChunkManager();

			float getChunkSpacing() const { return m_scale * Chunk::CHUNK_SIZE; }

			void loadChunk(const sf::Vector2f& pos);

			const std::vector<Chunk*>& getChunks(const sf::FloatRect &area) const;

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			private:
			struct ChunkGroup
			{
				sf::Vector2i position;
				Chunk* chunks[Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE];

				ChunkGroup()
				{
					memset(chunks, 0, sizeof(Chunk*) * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE);
				}

				Chunk* getChunk(size_t x, size_t y)
				{
					return chunks[y * Chunk::CHUNK_SIZE + x];
				}
			};
			
			std::unordered_map<sf::Vector2i, Chunk*, VectorHash> m_loadedChunks;
			std::unordered_map<sf::Vector2i, ChunkGroup*, VectorHash>m_chunkGroups;
			sf::IntRect m_generatedChunkBounds;


			mutable sf::FloatRect m_visibleArea;
			mutable std::vector<Chunk*> m_visibleChunks;
			mutable std::vector<ChunkGroup*> m_visibleChunkGroups;

			Chunk::Resources m_resources;
			float m_scale;
		};
	}
}