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

					static constexpr unsigned short shiftCount = (unsigned short)sizeof(int) * 8;
					long xMasked = ((static_cast<unsigned __int64>((long)v.x)) << shiftCount);
					long yMasked = (((long)v.y) & (static_cast<unsigned __int64>(~long(0)) >> shiftCount));
					// shorten the int range and combine both coordinates to one value
					hashed = xMasked | yMasked;
					return hashed;
				}
			};
			public:
			static Log::LogObject& getLogger();

			ChunkManager(const sf::Texture& texture, const sf::Vector2u& mapDim);
			~ChunkManager();

			void setChunkFactory(std::shared_ptr<Chunk::ChunkFactoryBase> factory)
			{
				m_chunkFactory = factory;
			}
			float getChunkSpacing() const { return m_scale * Chunk::CHUNK_SIZE; }

			void loadChunk(const sf::FloatRect& area, size_t numThreads = 1, bool async = false);
			void loadChunk(const sf::Vector2f& pos);
			

			const QSFML::vector<Chunk*>& getChunks(const sf::FloatRect &area) const;
			Chunk* getChunk(const sf::Vector2f& pos) const;
			const sf::IntRect& getLoadedChunkBounds() const { return m_generatedChunkBounds; }
			size_t getLoadedChunkCount() const { return m_loadedChunks.size(); }

			

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			private:
			
			static sf::Vector2i getChunkPosition(const sf::Vector2f& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkPos = sf::Vector2i(Chunk::CHUNK_SIZE * ((int)(pos.x + isNegX) / Chunk::CHUNK_SIZE),
												     Chunk::CHUNK_SIZE * ((int)(pos.y + isNegY) / Chunk::CHUNK_SIZE));


				if (pos.x < 0)
					chunkPos.x -= Chunk::CHUNK_SIZE;
				if (pos.y < 0)
					chunkPos.y -= Chunk::CHUNK_SIZE;

				/*sf::Vector2i chunkPos = sf::Vector2i(Chunk::CHUNK_SIZE * ((int)pos.x / Chunk::CHUNK_SIZE), Chunk::CHUNK_SIZE * ((int)pos.y / Chunk::CHUNK_SIZE));
				if (pos.x < 0 && chunkPos.x == 0)
					chunkPos.x -= Chunk::CHUNK_SIZE;
				if (pos.y < 0 && chunkPos.y == 0)
					chunkPos.y -= Chunk::CHUNK_SIZE;*/
				return chunkPos;
			}
			static sf::Vector2i getChunkPosition(const sf::Vector2i& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkPos = sf::Vector2i(Chunk::CHUNK_SIZE * ((pos.x + isNegX) / Chunk::CHUNK_SIZE),
													 Chunk::CHUNK_SIZE * ((pos.y + isNegY) / Chunk::CHUNK_SIZE));


				if (pos.x < 0)
					chunkPos.x -= Chunk::CHUNK_SIZE;
				if (pos.y < 0)
					chunkPos.y -= Chunk::CHUNK_SIZE;

				/*sf::Vector2i chunkPos = sf::Vector2i(Chunk::CHUNK_SIZE * (pos.x / Chunk::CHUNK_SIZE), Chunk::CHUNK_SIZE * (pos.y / Chunk::CHUNK_SIZE));
				if (pos.x < 0 && chunkPos.x == 0)
					chunkPos.x -= Chunk::CHUNK_SIZE;
				if (pos.y < 0 && chunkPos.y == 0)
					chunkPos.y -= Chunk::CHUNK_SIZE;*/
				return chunkPos;
			}
			static sf::Vector2i getChunkGroupPosition(const sf::Vector2f& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkGroupPos = sf::Vector2i(Chunk::CHUNK_SIZE_SQR * ((int)(pos.x + isNegX) / Chunk::CHUNK_SIZE_SQR),
					                                      Chunk::CHUNK_SIZE_SQR * ((int)(pos.y + isNegY) / Chunk::CHUNK_SIZE_SQR));
			

				if (pos.x < 0)
					chunkGroupPos.x -= Chunk::CHUNK_SIZE_SQR;
				if (pos.y < 0)
					chunkGroupPos.y -= Chunk::CHUNK_SIZE_SQR;
				return chunkGroupPos;
			}
			static sf::Vector2i getChunkGroupPosition(const sf::Vector2i& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkGroupPos = sf::Vector2i(Chunk::CHUNK_SIZE_SQR * ((pos.x + isNegX) / Chunk::CHUNK_SIZE_SQR),
					                                      Chunk::CHUNK_SIZE_SQR * ((pos.y + isNegY) / Chunk::CHUNK_SIZE_SQR));

				//if (isNegX)
				//	chunkGroupPos.x = Chunk::CHUNK_SIZE_SQR - chunkGroupPos.x - 1;
				//if (isNegY)
				//	chunkGroupPos.y = Chunk::CHUNK_SIZE_SQR - chunkGroupPos.y - 1;

				if (pos.x < 0)
					chunkGroupPos.x -= Chunk::CHUNK_SIZE_SQR;
				if (pos.y < 0)
					chunkGroupPos.y -= Chunk::CHUNK_SIZE_SQR;
				return chunkGroupPos;
			}
			static sf::Vector2i getRelativeChunkPosInGroup(const sf::Vector2i& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkIndex = sf::Vector2i((abs(pos.x + isNegX) / Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE,
													   (abs(pos.y + isNegY) / Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE);

				if (isNegX)
					chunkIndex.x = Chunk::CHUNK_SIZE - chunkIndex.x - 1;
				if (isNegY)
					chunkIndex.y = Chunk::CHUNK_SIZE - chunkIndex.y - 1;
#ifdef QSFML_DEBUG
				if (chunkIndex.x >= Chunk::CHUNK_SIZE || chunkIndex.y >= Chunk::CHUNK_SIZE)
					getLogger().logError("getRelativeChunkPosInGroup: Index out of bounds");
#endif
				return chunkIndex;
			}
			static sf::Vector2i getRelativeChunkPosInGroup(const sf::Vector2f& pos)
			{
				bool isNegX = pos.x < 0;
				bool isNegY = pos.y < 0;

				sf::Vector2i chunkIndex = sf::Vector2i(((int)abs(pos.x + (float)isNegX) / Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE,
													   ((int)abs(pos.y + (float)isNegY) / Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE);

				if (isNegX)
					chunkIndex.x = Chunk::CHUNK_SIZE - chunkIndex.x - 1;
				if (isNegY)
					chunkIndex.y = Chunk::CHUNK_SIZE - chunkIndex.y - 1;
#ifdef QSFML_DEBUG
				if (chunkIndex.x >= Chunk::CHUNK_SIZE || chunkIndex.y >= Chunk::CHUNK_SIZE)
					getLogger().logError("getRelativeChunkPosInGroup: Index out of bounds");
#endif
				return chunkIndex;
			}
			static size_t getRelativeChunkIndexInGroup(const sf::Vector2i& relativePos)
			{
#ifdef QSFML_DEBUG
				size_t pos = relativePos.y * Chunk::CHUNK_SIZE + relativePos.x;
				if (pos >= Chunk::CHUNK_SIZE_SQR)
					getLogger().logError("getRelativeChunkIndexInGroup: Index out of bounds");
#endif
				return relativePos.y * Chunk::CHUNK_SIZE + relativePos.x;
			}



			struct ChunkGroup
			{
				sf::Vector2i position;
				Chunk* chunks[Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE];
				sf::VertexArray m_vertexBuffer;
				//Chunk m_lowResChunk;

				ChunkGroup(const sf::Vector2i& pos);

				Chunk* getChunk(size_t x, size_t y)
				{
#ifdef QSFML_DEBUG
					if (x >= Chunk::CHUNK_SIZE || y >= Chunk::CHUNK_SIZE)
					{
						getLogger().logError("ChunkGroup::getChunk: Index out of bounds");
						return nullptr;
					}
#endif
					return chunks[y * Chunk::CHUNK_SIZE + x];
				}

				void updateLowResMap();

				void draw(sf::RenderTarget& target, const sf::RenderStates &states) const
				{
					target.draw(m_vertexBuffer, states);
				}


				
			};
			const QSFML::vector<ChunkGroup*>& getChunkGroups(const sf::FloatRect& area) const;
			void updateGeneratedChunkBounds(const QSFML::vector<Chunk*>& newChunks);
			void updateGeneratedChunkBounds(const Chunk*& newChunk);
			void insertNewChunk(Chunk*& chunk);

			std::unordered_map<sf::Vector2i, Chunk*, VectorHash> m_loadedChunks;
			QSFML::vector<Chunk*> m_loadedChunksVec;
			std::unordered_map<sf::Vector2i, ChunkGroup*, VectorHash>m_chunkGroups;
			sf::IntRect m_generatedChunkBounds;


			mutable bool m_needsDrawUpdate;
			mutable sf::FloatRect m_visibleArea;
			mutable QSFML::vector<Chunk*> m_visibleChunks;
			mutable QSFML::vector<ChunkGroup*> m_visibleChunkGroups;
#ifdef QSFML_DEBUG
			mutable QSFML::vector<sf::FloatRect> m_visibleChunkGroupBounds;
#endif

			Chunk::Resources m_resources;
			float m_scale;

			struct AsyncChunkLoaderData
			{
				std::atomic<bool> stop;
				std::vector<std::thread*> threads;
				QSFML::vector<Chunk*> loadedChunks;
			};
			std::mutex m_mutex;
			std::atomic<int> m_threadsLoading;

			std::vector<AsyncChunkLoaderData*> m_asyncChunkLoaderData;

			std::shared_ptr<Chunk::ChunkFactoryBase> m_chunkFactory = nullptr;
		};
	}
}