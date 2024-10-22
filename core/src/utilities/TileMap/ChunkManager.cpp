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
		{
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
			sf::Vector2i chunkPos = sf::Vector2i((int)Chunk::CHUNK_SIZE*((int)pos.x / (int)Chunk::CHUNK_SIZE), Chunk::CHUNK_SIZE*((int)pos.y / (int)Chunk::CHUNK_SIZE));
			auto it = m_loadedChunks.find(chunkPos);
			if (it == m_loadedChunks.end())
			{
#ifdef QSFML_DEBUG
				getLogger().logInfo("loadChunk(): Pos: {" + std::to_string(chunkPos.x) + "," + std::to_string(chunkPos.y) + "} "
									"Loading new chunk");
#endif
				std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkPos, m_resources);
				chunk->generate();
				chunk->updateTextureCoords();
				m_loadedChunks.insert(std::make_pair(chunkPos, chunk));
			}
		}


		void ChunkManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			const sf::View& view = target.getView();
			float zoom = view.getSize().x / target.getSize().x;


			if (zoom < 0.4)
			{
				states.texture = &m_resources.textureMap.getTexture();
				for (auto& chunk : m_loadedChunks)
				{
					chunk.second->draw_LOD_high(target, states);
				}
			}
			else
			{
				for (auto& chunk : m_loadedChunks)
				{
					chunk.second->draw_LOD_low(target, states);
				}
			}
		}
	}
}