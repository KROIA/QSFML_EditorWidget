#pragma once

#include "QSFML_EditorWidget_base.h"
#include "SFML/Graphics/Texture.hpp"

namespace QSFML
{
	namespace Assets
	{
		class QSFML_EDITOR_WIDGET_EXPORT TextureMap
		{
			public:
			struct UVMapCoords
			{
				sf::Vector2f topLeft;
				sf::Vector2f topRight;
				sf::Vector2f bottomLeft;
				sf::Vector2f bottomRight;

				sf::IntRect toIntRect() const
				{
					return sf::IntRect(topLeft.x, topLeft.y,
									   topRight.x - topLeft.x,
									   bottomLeft.y - topLeft.y);
				}
			};

			TextureMap(const sf::Texture& texture,
					   const sf::Vector2u& mapDim,
					   float padding = 0);

			TextureMap(const TextureMap& other);
			TextureMap(const TextureMap&& other);
			~TextureMap();

			void setPadding(float padding);
			float getPadding() const { return m_padding; }

			size_t getImageCount() const { return m_mapDim.x * m_mapDim.y; }
			size_t getIndex(size_t x, size_t y) const
			{
				return y * m_mapDim.x + x;
			}
			size_t getIndex(const sf::Vector2u& pos) const
			{
				return getIndex(pos.x, pos.y);
			}
			sf::Vector2u getPos(size_t idx) const
			{
				return sf::Vector2u(idx % m_mapDim.x, idx / m_mapDim.x);
			}
			const sf::Vector2f& getSubImageSize() const
			{
				return m_subImageSize;
			}

			const sf::Texture& getTexture() const { return m_texture; }
			const sf::Vector2u& getMapDim() const { return m_mapDim; }

			const UVMapCoords& getUVMapCoords(size_t idx) const;
			const UVMapCoords& getUVMapCoords(const sf::Vector2u& pos) const;


			private:
			void bakeUVMapCoords();

			const sf::Texture& m_texture;
			const sf::Vector2u m_mapDim;

			float m_padding;

			sf::Vector2f m_subImageSize;
			std::vector<UVMapCoords> m_uvMapCoords;
		};
	}
}