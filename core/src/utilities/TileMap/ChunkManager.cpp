#include "utilities/TileMap/ChunkManager.h"

namespace QSFML
{
	namespace Utilities
	{
		Log::LogObject& ChunkManager::getLogger()
		{
			static Log::LogObject logger = Log::LogObject("ChunkManager");
			return logger;
		}
		ChunkManager::ChunkManager(const sf::Texture& texture, const sf::Vector2u& mapDim)
			: m_resources{ Assets::TextureMap(texture, mapDim, 0.1) }
			, m_scale(1)
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			// Calculate the low level color
			sf::Image image = texture.copyToImage();
			m_resources.lowLevelReplacementColors.resize(mapDim.x * mapDim.y);
			sf::Vector2u subSize = sf::Vector2u(m_resources.textureMap.getSubImageSize());
			
			for (size_t y = 0; y < mapDim.y; y++)
			{
				for (size_t x = 0; x < mapDim.x; x++)
				{
					float r = 0, g = 0, b = 0, a = 0;
					// calculate the average color of the sub image
					for (size_t i = 0; i < subSize.y; i++)
					{
						for (size_t j = 0; j < subSize.x; j++)
						{
							sf::Color color = image.getPixel(x * subSize.x + j, y * subSize.y + i);
							r += color.r;
							g += color.g;
							b += color.b;
							a += color.a;
						}
					}
					r /= subSize.x * subSize.y;
					g /= subSize.x * subSize.y;
					b /= subSize.x * subSize.y;
					a /= subSize.x * subSize.y;
					m_resources.lowLevelReplacementColors[y * mapDim.x + x] = sf::Color(r, g, b, a);
				}
			}
		}
		ChunkManager::~ChunkManager()
		{

		}

		void ChunkManager::loadChunk(const sf::Vector2f& pos)
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			sf::Vector2i chunkPos = sf::Vector2i(((int)(Chunk::CHUNK_SIZE * pos.x/ m_scale) / (int)Chunk::CHUNK_SIZE),
												 ((int)(Chunk::CHUNK_SIZE * pos.y/ m_scale) / (int)Chunk::CHUNK_SIZE));
			auto it = m_loadedChunks.find(chunkPos);
			if (it == m_loadedChunks.end())
			{
#ifdef QSFML_DEBUG
				getLogger().logInfo("loadChunk(): Pos: {" + std::to_string(chunkPos.x) + "," + std::to_string(chunkPos.y) + "} "
									"Loading new chunk");
#endif
				Chunk* chunk = new Chunk(chunkPos, m_resources, m_scale);
				chunk->generate();
				chunk->updateTextureCoords();
				m_loadedChunks.insert(std::make_pair(chunkPos, chunk));

				if (chunkPos.x < m_generatedChunkBounds.left)
				{
					m_generatedChunkBounds.width += m_generatedChunkBounds.left - chunkPos.x;
					m_generatedChunkBounds.left = chunkPos.x;
				}
				else if (chunkPos.x >= m_generatedChunkBounds.left + m_generatedChunkBounds.width)
				{
					m_generatedChunkBounds.width = chunkPos.x - m_generatedChunkBounds.left + 1;
				}

				if (chunkPos.y < m_generatedChunkBounds.top)
				{
					m_generatedChunkBounds.height += m_generatedChunkBounds.top - chunkPos.y;
					m_generatedChunkBounds.top = chunkPos.y;
				}
				else if (chunkPos.y >= m_generatedChunkBounds.top + m_generatedChunkBounds.height)
				{
					m_generatedChunkBounds.height = chunkPos.y - m_generatedChunkBounds.top + 1;
				}
				if (m_visibleArea.contains(pos))
				{
					m_visibleChunks.push_back(chunk);
				}

				sf::Vector2i chunkGroupPos = sf::Vector2i(chunkPos.x / Chunk::CHUNK_SIZE, chunkPos.y / Chunk::CHUNK_SIZE);
				auto groupIt = m_chunkGroups.find(chunkGroupPos);
				if (groupIt == m_chunkGroups.end())
				{
					ChunkGroup* group = new ChunkGroup();
					group->chunks[(chunkPos.y % Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE + (chunkPos.x % Chunk::CHUNK_SIZE)] = chunk;
					group->position = chunkGroupPos;
					m_chunkGroups.insert(std::make_pair(chunkGroupPos, group));
				}
				else
				{
					groupIt->second->chunks[(chunkPos.y % Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE + (chunkPos.x % Chunk::CHUNK_SIZE)] = chunk;
				}
			}
		}


		const std::vector<Chunk*>& ChunkManager::getChunks(const sf::FloatRect& area) const
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			if (area == m_visibleArea)
			{
				return m_visibleChunks;
			}
			m_visibleArea = area;
			size_t lastSize = m_visibleChunks.size();
			m_visibleChunks.clear();
			m_visibleChunks.reserve(lastSize*2);
			m_visibleChunkGroups.clear();
			m_visibleChunkGroups.reserve(16);

			sf::Vector2i start = sf::Vector2i(((int)(Chunk::CHUNK_SIZE * area.left / m_scale) / (int)Chunk::CHUNK_SIZE),
											  ((int)(Chunk::CHUNK_SIZE * area.top / m_scale) / (int)Chunk::CHUNK_SIZE));

			sf::Vector2i end = sf::Vector2i(((int)(Chunk::CHUNK_SIZE * (area.left + area.width) / m_scale) / (int)Chunk::CHUNK_SIZE),
											((int)(Chunk::CHUNK_SIZE * (area.top + area.height) / m_scale) / (int)Chunk::CHUNK_SIZE));


			// Take min range from ChunkBounds
			start.x = std::max(start.x, m_generatedChunkBounds.left);
			start.y = std::max(start.y, m_generatedChunkBounds.top);
			end.x = std::min(end.x, m_generatedChunkBounds.left + m_generatedChunkBounds.width - 1);
			end.y = std::min(end.y, m_generatedChunkBounds.top + m_generatedChunkBounds.height - 1);

			// Find relevant ChunkGroups
			for (int x = start.x / Chunk::CHUNK_SIZE; x <= end.x / Chunk::CHUNK_SIZE; x++)
			{
				for (int y = start.y / Chunk::CHUNK_SIZE; y <= end.y / Chunk::CHUNK_SIZE; y++)
				{
					QSFMLP_GENERAL_BLOCK("Find chunk group", QSFML_COLOR_STAGE_2);
					const auto it = m_chunkGroups.find(sf::Vector2i(x, y));
					if (it != m_chunkGroups.end())
					{
						m_visibleChunkGroups.push_back(it->second);
					}
				}
			}

			// Search in chunk groups
			for (auto& group : m_visibleChunkGroups)
			{
				sf::Vector2i startPos = sf::Vector2i(std::max(start.x, group->position.x * Chunk::CHUNK_SIZE),
													 std::max(start.y, group->position.y * Chunk::CHUNK_SIZE));

				sf::Vector2i endPos = sf::Vector2i(std::min(end.x, (group->position.x + 1) * Chunk::CHUNK_SIZE - 1),
												   std::min(end.y, (group->position.y + 1) * Chunk::CHUNK_SIZE - 1));

				for (int x = startPos.x; x <= endPos.x; x++)
				{
					for (int y = startPos.y; y <= endPos.y; y++)
					{
						QSFMLP_GENERAL_BLOCK("Find chunk", QSFML_COLOR_STAGE_2);
						Chunk* chunk = group->getChunk(x % Chunk::CHUNK_SIZE, y % Chunk::CHUNK_SIZE);
						if (chunk)
						{
							m_visibleChunks.push_back(chunk);
						}
					}
				}
			}

			/*
			for (int x = start.x; x <= end.x; x++)
			{
				for (int y = start.y; y <= end.y; y++)
				{
					QSFMLP_GENERAL_BLOCK("Find chunk", QSFML_COLOR_STAGE_2);
					auto it = m_loadedChunks.find(sf::Vector2i(x, y));
					if (it != m_loadedChunks.end())
					{
						m_visibleChunks.push_back(it->second);
					}
				}
			}*/
			return m_visibleChunks;
		}

		void ChunkManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			const sf::View& view = target.getView();
			float zoom = view.getSize().x / target.getSize().x;

			sf::FloatRect viewRect = sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

			const std::vector<Chunk*>& chunks = getChunks(viewRect);
			if (zoom < 0.2)
			{
				states.texture = &m_resources.textureMap.getTexture();
				for (auto& chunk : chunks)
				{
					chunk->draw_LOD_high(target, states);
				}
			}
			else
			{
				for (auto& chunk : chunks)
				{
					chunk->draw_LOD_low(target, states);
				}
			}
			//getLogger().logInfo("Visible chunks: " + std::to_string(chunks.size()));
		}
	}
}