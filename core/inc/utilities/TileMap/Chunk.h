#pragma once

#include "QSFML_EditorWidget_base.h"
#include "ChunkData.h"
#include "assets/TextureMap.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <type_traits> // For std::is_base_of


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
			private:
			friend class ChunkManager;
			class ChunkFactoryBase
			{
				// Ensure T is derived from Base at compile-time

				public:
				virtual ~ChunkFactoryBase() {}

				// User can override this to define how to create the object
				virtual Chunk* create(const sf::Vector2i& pos, const Resources& resources, float scale) const
				{
					return new Chunk(pos, resources, scale);
				}
			};

			public:
			

			template<typename T>
			class ChunkFactory : public ChunkFactoryBase 
			{
				// Ensure T is derived from Base at compile-time
				static_assert(std::is_base_of<Chunk, T>::value, "T must be a Chunk type");
				public:
				virtual ~ChunkFactory() {}

				// User can override this to define how to create the object
				virtual Chunk* create(const sf::Vector2i& pos, const Resources& resources, float scale) const override
				{
					return new T(pos, resources, scale);
				}
			};

			template<typename T>
			static std::shared_ptr<ChunkFactoryBase> createFactory()
			{
				return std::make_shared<ChunkFactory<T>>();
			}
			
			static constexpr int CHUNK_SIZE = ChunkData::CHUNK_SIZE;
			static constexpr int CHUNK_SIZE_SQR = CHUNK_SIZE * CHUNK_SIZE;
			static constexpr int CHUNK_LOW_RES_SIZE = CHUNK_SIZE / 4;
			static Log::LogObject& getLogger();

			Chunk(const sf::Vector2i& pos, const Resources& resources, float scale = 1);
			virtual ~Chunk();

			const sf::Vector2i& getPosition() const { return m_position; }
			bool isLoaded() const { return m_loaded; }
			sf::IntRect getBounds() const { return sf::IntRect(m_position, sf::Vector2i(CHUNK_SIZE, CHUNK_SIZE)); }
			static sf::IntRect getBounds(const QSFML::vector<Chunk*>& chunks);
			static sf::IntRect getBounds(const sf::IntRect& A, const sf::IntRect &B);

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

			sf::VertexArray m_lowResVertexBuffer;
			
		};
	}
}