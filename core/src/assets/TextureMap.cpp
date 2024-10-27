#include "assets/TextureMap.h"

namespace QSFML
{
	namespace Assets
	{
		TextureMap::TextureMap(const sf::Texture& texture,
							   const sf::Vector2u& mapDim,
							   float padding)
			: m_texture(texture)
			, m_mapDim(mapDim)
			, m_padding(padding)
		{
			bakeUVMapCoords();
		}
		TextureMap::TextureMap(const TextureMap& other)
			: m_texture(other.m_texture)
			, m_mapDim(other.m_mapDim)
			, m_padding(other.m_padding)
			, m_subImageSize(other.m_subImageSize)
			, m_uvMapCoords(other.m_uvMapCoords)
		{

		}
		TextureMap::TextureMap(const TextureMap&& other)
			: m_texture(other.m_texture)
			, m_mapDim(std::move(other.m_mapDim))
			, m_padding(std::move(other.m_padding))
			, m_subImageSize(std::move(other.m_subImageSize))
			, m_uvMapCoords(std::move(other.m_uvMapCoords))
		{

		}
		TextureMap::~TextureMap()
		{

		}

		void TextureMap::setPadding(float padding)
		{
			m_padding = padding;
			bakeUVMapCoords();
		}


		const TextureMap::UVMapCoords& TextureMap::getUVMapCoords(size_t idx) const
		{
			return m_uvMapCoords[idx];
		}
		const TextureMap::UVMapCoords& TextureMap::getUVMapCoords(const sf::Vector2u& pos) const
		{
			return m_uvMapCoords[getIndex(pos)];
		}

		void TextureMap::bakeUVMapCoords()
		{
			m_uvMapCoords.resize(m_mapDim.x * m_mapDim.y);
			m_subImageSize = sf::Vector2f(m_texture.getSize().x / m_mapDim.x, m_texture.getSize().y / m_mapDim.y);
			
			for (size_t y = 0; y < m_mapDim.y; y++)
			{
				for (size_t x = 0; x < m_mapDim.x; x++)
				{
					size_t idx = getIndex(x, y);
					
					UVMapCoords& uvMapCoords = m_uvMapCoords[idx];
					uvMapCoords.topLeft.x = (x * m_subImageSize.x) + m_padding;
					uvMapCoords.topLeft.y = (y * m_subImageSize.y) + m_padding;
					uvMapCoords.topRight.x = (x * m_subImageSize.x) + m_subImageSize.x - m_padding;
					uvMapCoords.topRight.y = uvMapCoords.topLeft.y;
					uvMapCoords.bottomRight.x = uvMapCoords.topRight.x;
					uvMapCoords.bottomRight.y = (y * m_subImageSize.y) + m_subImageSize.y - m_padding;
					uvMapCoords.bottomLeft.x = uvMapCoords.topLeft.x;
					uvMapCoords.bottomLeft.y = uvMapCoords.bottomRight.y;
				}
			}
		}

	}
}