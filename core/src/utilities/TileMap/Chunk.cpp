#include "utilities/TileMap/Chunk.h"
#include "utilities/PerlinNoise.h"


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
			, m_vertexBuffer(sf::PrimitiveType::Quads, CHUNK_SIZE_SQR * 4)
			, m_lowResVertexBuffer(sf::PrimitiveType::Quads, CHUNK_LOW_RES_SIZE* CHUNK_LOW_RES_SIZE*4)
		{
			
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
		
			float chunkSize2 = (float)CHUNK_SIZE/(float)CHUNK_LOW_RES_SIZE;
			for (int x = 0; x < CHUNK_LOW_RES_SIZE; x++)
			{
				for (int y = 0; y < CHUNK_LOW_RES_SIZE; y++)
				{
					int i = (y * CHUNK_LOW_RES_SIZE + x) * 4;
					m_lowResVertexBuffer[i].position = sf::Vector2f(m_position.x + x * chunkSize2, m_position.y + y * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 1].position = sf::Vector2f(m_position.x + (x + 1) * chunkSize2, m_position.y + y * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 2].position = sf::Vector2f(m_position.x + (x + 1) * chunkSize2, m_position.y + (y + 1) * chunkSize2) * scale;
					m_lowResVertexBuffer[i + 3].position = sf::Vector2f(m_position.x + x * chunkSize2, m_position.y + (y + 1) * chunkSize2) * scale;

				}
			}
		}

		Chunk::~Chunk()
		{
			QSFMLP_GENERAL_FUNCTION(QSFML_COLOR_STAGE_1);
			QSFMLP_GENERAL_BLOCK("m_lowResVertexBuffer.clear()", QSFML_COLOR_STAGE_2);
			m_lowResVertexBuffer.clear();
			QSFMLP_GENERAL_END_BLOCK;
			QSFMLP_GENERAL_BLOCK("m_vertexBuffer.clear()", QSFML_COLOR_STAGE_2);
			m_vertexBuffer.clear();
			QSFMLP_GENERAL_END_BLOCK;
			QSFMLP_GENERAL_BLOCK("delete m_chunkData",QSFML_COLOR_STAGE_2);
			if (m_chunkData)
				delete m_chunkData;
			QSFMLP_GENERAL_END_BLOCK;
		}

		sf::IntRect Chunk::getBounds(const QSFML::vector<Chunk*>& chunks)
		{
			sf::IntRect bounds;
			if (chunks.empty())
				return bounds;
			for (const auto& chunk : chunks)
			{
				sf::IntRect chunkBounds = chunk->getBounds();
				if (chunkBounds.left < bounds.left)
				{
					bounds.width += bounds.left - chunkBounds.left;
					bounds.left = chunkBounds.left;
				}
				if (chunkBounds.top < bounds.top)
				{
					bounds.height += bounds.top - chunkBounds.top;
					bounds.top = chunkBounds.top;
				}
				if (chunkBounds.left + chunkBounds.width > bounds.left + bounds.width)
					bounds.width = chunkBounds.left + chunkBounds.width - bounds.left;
				if (chunkBounds.top + chunkBounds.height > bounds.top + bounds.height)
					bounds.height = chunkBounds.top + chunkBounds.height - bounds.top;
			}
			return bounds;
		}
		sf::IntRect Chunk::getBounds(const sf::IntRect& A, const sf::IntRect& B)
		{
			sf::IntRect bounds;
			bounds.left = std::min(A.left, B.left);
			bounds.top = std::min(A.top, B.top);
			bounds.width = std::max(A.left + A.width, B.left + B.width) - bounds.left;
			bounds.height = std::max(A.top + A.height, B.top + B.height) - bounds.top;
			return bounds;
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
			updateTextureCoords();
			m_loaded = true;
		}
		void Chunk::onGenerate()
		{
			m_chunkData = new ChunkData();		
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

#ifdef QSFML_DEBUG
			// Draw border lines
			sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
			lines[0].position = sf::Vector2f(m_position.x, m_position.y);
			lines[1].position = sf::Vector2f(m_position.x + CHUNK_SIZE, m_position.y);
			lines[2].position = sf::Vector2f(m_position.x + CHUNK_SIZE, m_position.y + CHUNK_SIZE);
			lines[3].position = sf::Vector2f(m_position.x, m_position.y + CHUNK_SIZE);
			lines[4].position = lines[0].position;

			sf::Color color(200, 200, 0);
			lines[0].color = color;
			lines[1].color = color;
			lines[2].color = color;
			lines[3].color = color;
			lines[4].color = color;
			target.draw(lines);
#endif
		}

		void Chunk::draw_LOD_low(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_lowResVertexBuffer, states);

#ifdef QSFML_DEBUG
			// Draw border lines
			sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5);
			lines[0].position = sf::Vector2f(m_position.x, m_position.y);
			lines[1].position = sf::Vector2f(m_position.x + CHUNK_SIZE, m_position.y);
			lines[2].position = sf::Vector2f(m_position.x + CHUNK_SIZE, m_position.y + CHUNK_SIZE);
			lines[3].position = sf::Vector2f(m_position.x, m_position.y + CHUNK_SIZE);
			lines[4].position = lines[0].position;

			sf::Color color(200,50,0);
			lines[0].color = color;
			lines[1].color = color;
			lines[2].color = color;
			lines[3].color = color;
			lines[4].color = color;
			target.draw(lines);
#endif
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

			struct ColorF
			{
				float r = 0, g = 0, b = 0, a = 0;
			};

			ColorF color[CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE];

			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = 0; y < CHUNK_SIZE; y++)
				{
					size_t idx = (y * CHUNK_SIZE + x) * 4;
					size_t textureIndex = textureIdx[y * CHUNK_SIZE + x];
					const Assets::TextureMap::UVMapCoords& uvMapCoords = m_resources.textureMap.getUVMapCoords(textureIndex);
					m_vertexBuffer[idx].texCoords = uvMapCoords.topLeft;
					m_vertexBuffer[idx + 1].texCoords = uvMapCoords.topRight;
					m_vertexBuffer[idx + 2].texCoords = uvMapCoords.bottomRight;
					m_vertexBuffer[idx + 3].texCoords = uvMapCoords.bottomLeft;

					size_t colIndex = (y * CHUNK_LOW_RES_SIZE / CHUNK_SIZE) * CHUNK_LOW_RES_SIZE + (x * CHUNK_LOW_RES_SIZE / CHUNK_SIZE);
					color[colIndex].r += m_resources.lowLevelReplacementColors[textureIndex].r;
					color[colIndex].g += m_resources.lowLevelReplacementColors[textureIndex].g;
					color[colIndex].b += m_resources.lowLevelReplacementColors[textureIndex].b;
					color[colIndex].a += m_resources.lowLevelReplacementColors[textureIndex].a;

				}
			}

			float divisor = (float)(CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE) / (CHUNK_SIZE * CHUNK_SIZE);

			sf::Color col;
			ColorF col1;
			for (size_t i = 0; i < CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE; i++)
			{
				color[i].r *= divisor;
				color[i].g *= divisor;
				color[i].b *= divisor;
				color[i].a *= divisor;

				int index = i * 4;
				sf::Color c(color[i].r, color[i].g, color[i].b, color[i].a);
				m_lowResVertexBuffer[index].color = c;
				m_lowResVertexBuffer[index + 1].color = c;
				m_lowResVertexBuffer[index + 2].color = c;
				m_lowResVertexBuffer[index + 3].color = c;

				col1.r += color[i].r;
				col1.g += color[i].g;
				col1.b += color[i].b;
				col1.a += color[i].a;
			}
			col.r = (sf::Uint8)(col1.r / (float)(CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE));
			col.g = (sf::Uint8)(col1.g / (float)(CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE));
			col.b = (sf::Uint8)(col1.b / (float)(CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE));
			col.a = (sf::Uint8)(col1.a / (float)(CHUNK_LOW_RES_SIZE * CHUNK_LOW_RES_SIZE));


			m_chunkData->setLowLevelReplacementColor(col);
		}
	}
}