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

			void loadChunk(const sf::Vector2f& pos);

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			private:
			std::unordered_map<sf::Vector2i, std::shared_ptr<Chunk>, VectorHash> m_loadedChunks;
			
			Chunk::Resources m_resources;

		};
	}
}