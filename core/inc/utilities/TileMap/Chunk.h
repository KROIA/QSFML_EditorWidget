#pragma once

#include "QSFML_EditorWidget_base.h"
#include "ChunkData.h"
#include "assets/TextureMap.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace QSFML
{
	namespace Utilities
	{ 
	    class QSFML_EDITOR_WIDGET_EXPORT Chunk
		{
			public:
			struct Resources
			{
				Assets::TextureMap textureMap;
				std::vector<sf::Color> lowLevelReplacementColors;
			};
			static constexpr int CHUNK_SIZE = 16;
			static Log::LogObject& getLogger();

			Chunk(const sf::Vector2i& pos, const Resources& resources);
			virtual ~Chunk();

			const sf::Vector2i& getPosition() const { return m_position; }
			
			void generate();
			void updateTextureCoords();
			//virtual void load();
			//virtual void save();

			ChunkData* getChunkData() { return m_chunkData; }
			const ChunkData* getChunkData() const { return m_chunkData; }
			
			void draw_LOD_high(sf::RenderTarget& target, sf::RenderStates states) const;
			void draw_LOD_low(sf::RenderTarget& target, sf::RenderStates states) const;


			protected:
			virtual void onGenerate();


			ChunkData* m_chunkData;


			private:
			
			const Resources& m_resources;

			bool m_loaded;
			sf::Vector2i m_position;
			sf::VertexArray m_vertexBuffer;
			
		};
	}
}