#include "utilities/TileMap/Chunk.h"
#include "utilities/PerlinNoise.h"
#include "FastNoiseLite.h"

namespace QSFML
{
	namespace Utilities
	{
		Log::LogObject& Chunk::getLogger()
		{
			static Log::LogObject logger = Log::LogObject("Chunk");
			return logger;
		}
		Chunk::Chunk(const sf::Vector2i& pos, const Resources& resources, float scale)
			: m_resources(resources)
			, m_chunkData(nullptr)
			, m_loaded(false)
			, m_position(pos)
			, m_vertexBuffer(sf::PrimitiveType::Quads)
			, m_lowResVertexBuffer(sf::PrimitiveType::Quads)
		{
			m_vertexBuffer.resize(CHUNK_SIZE * CHUNK_SIZE * 4);
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					size_t idx = (y * CHUNK_SIZE + x) * 4;
				//	sf::Vertex* quad = &m_vertexBuffer[idx];
					m_vertexBuffer[idx].position = sf::Vector2f(x + m_position.x, y + m_position.y) * scale;
					m_vertexBuffer[idx + 1].position = sf::Vector2f(x + 1 + m_position.x, y + m_position.y) * scale;
					m_vertexBuffer[idx + 2].position = sf::Vector2f(x + 1 + m_position.x, y + 1 + m_position.y) * scale;
					m_vertexBuffer[idx + 3].position = sf::Vector2f(x + m_position.x, y + 1 + m_position.y) * scale;

					m_vertexBuffer[idx].texCoords = sf::Vector2f(0, 0);
					m_vertexBuffer[idx + 1].texCoords = sf::Vector2f(1, 0);
					m_vertexBuffer[idx + 2].texCoords = sf::Vector2f(1, 1);
					m_vertexBuffer[idx + 3].texCoords = sf::Vector2f(0, 1);
				}
			}
			m_lowResVertexBuffer.resize(4*4);
			float chunkSize2 = CHUNK_SIZE *0.5f;
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					int i = (y * 2 + x) * 4;
					m_lowResVertexBuffer[i].position = sf::Vector2f(m_position.x + x * chunkSize2, m_position.y + y * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 1].position = sf::Vector2f(m_position.x + (x + 1) * chunkSize2, m_position.y + y * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 2].position = sf::Vector2f(m_position.x + (x + 1) * chunkSize2, m_position.y + (y + 1) * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 3].position = sf::Vector2f(m_position.x + x * chunkSize2, m_position.y + (y + 1) * chunkSize2) * scale;

				}
			}
		}

		Chunk::~Chunk()
		{
			if (m_chunkData)
				delete m_chunkData;
		}

		void Chunk::generate()
		{
			if (m_loaded)
			{
				getLogger().logWarning("generate(): Pos: {" + std::to_string(m_position.x) + "," + std::to_string(m_position.y) + "} "
									   "Chunk already generated");
				return;
			}
			onGenerate();
			if (m_chunkData)
				m_chunkData->setPosition(m_position);
			else
			{
				getLogger().logError("generate(): Pos: {"+std::to_string(m_position.x)+","+std::to_string(m_position.y)+"} "
									 "Chunk::m_chunkData is not initialized");
			}
			m_loaded = true;
		}
		void Chunk::onGenerate()
		{
			m_chunkData = new ChunkData();
			
			//static Utilities::PerlinNoise noise;
			static FastNoiseLite noise;
			noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

			float scale = 1;

			struct ColorF
			{
				float r = 0 , g=0, b=0, a=0;
			};
			ColorF color[4];
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = 0; y < CHUNK_SIZE; y++)
				{
					sf::Vector2f pos = sf::Vector2f(((float)x + m_position.x)*scale, ((float)y + m_position.y)*scale);
					//float value = noise.noise(pos.x, pos.y, 5, sf::Vector2u(100, 100))+0.5f;
					float value = noise.GetNoise(pos.x, pos.y);


					size_t textureIndex = 0;

					
					if (value < 0.2f)
						textureIndex = 2;
					else if (value < 0.4f)
						textureIndex = 3;
					else if (value < 0.6f)
						textureIndex = 2;
					else if (value < 0.7f)
						textureIndex = 4;



					m_chunkData->setTextureIndex(x, y, textureIndex);
					int colIndex = (y *2 / CHUNK_SIZE) * 2 + (x*2 / CHUNK_SIZE);
					color[colIndex].r += m_resources.lowLevelReplacementColors[textureIndex].r;
					color[colIndex].g += m_resources.lowLevelReplacementColors[textureIndex].g;
					color[colIndex].b += m_resources.lowLevelReplacementColors[textureIndex].b;
					color[colIndex].a += m_resources.lowLevelReplacementColors[textureIndex].a;
					
				}
			}
			float divisor = 4.f / (CHUNK_SIZE * CHUNK_SIZE);

			sf::Color col;
			ColorF col1;
			for (int i = 0; i < 4; i++)
			{
				color[i].r *= divisor;
				color[i].g *= divisor;
				color[i].b *= divisor;
				color[i].a *= divisor;

				int index = i * 4;
				sf::Color c(color[i].r, color[i].g, color[i].b, color[i].a);
				m_lowResVertexBuffer[index].color = c;
				m_lowResVertexBuffer[index +1].color = c;
				m_lowResVertexBuffer[index +2].color = c;
				m_lowResVertexBuffer[index +3].color = c;

				col1.r += color[i].r;
				col1.g += color[i].g;
				col1.b += color[i].b;
				col1.a += color[i].a;
			}
			col.r = (sf::Uint8)(col1.r / 4.f);
			col.g = (sf::Uint8)(col1.g / 4.f);
			col.b = (sf::Uint8)(col1.b / 4.f);
			col.a = (sf::Uint8)(col1.a / 4.f);
			
			
			m_chunkData->setLowLevelReplacementColor(col);
			
		}


		void Chunk::draw_LOD_high(sf::RenderTarget& target, sf::RenderStates states) const
		{
			/*if (!m_chunkData)
			{
				getLogger().logError("draw_LOD_high(): Pos: {" + std::to_string(m_position.x) + "," + std::to_string(m_position.y) + "} "
									 "Chunk is not loaded");
				return;
			}
			if (!m_loaded)
			{
				getLogger().logError("draw_LOD_high(): Pos: {" + std::to_string(m_position.x) + "," + std::to_string(m_position.y) + "} "
									 "Chunk is not generated");
				return;
			}*/
			//updateTextureCoords();
			//states.texture = &m_textureMap.getTexture();
			target.draw(m_vertexBuffer, states);
		}

		void Chunk::draw_LOD_low(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_lowResVertexBuffer, states);
		}


		void Chunk::updateTextureCoords()
		{
			if (!m_chunkData)
			{
				getLogger().logError("updateTextureCoords(): Pos: {" + std::to_string(m_position.x) + "," + std::to_string(m_position.y) + "} "
									 "Chunk is not loaded");
				return;
			}
			const std::vector<size_t>& textureIdx = m_chunkData->getTextureIndex();
			for (size_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
			{
				const size_t& textureIndex = textureIdx[i];
				size_t idx = i * 4;
				const Assets::TextureMap::UVMapCoords& uvMapCoords = m_resources.textureMap.getUVMapCoords(textureIndex);
				m_vertexBuffer[idx].texCoords = uvMapCoords.topLeft;
				m_vertexBuffer[idx + 1].texCoords = uvMapCoords.topRight;
				m_vertexBuffer[idx + 2].texCoords = uvMapCoords.bottomRight;
				m_vertexBuffer[idx + 3].texCoords = uvMapCoords.bottomLeft;
			}
		}
	}
}