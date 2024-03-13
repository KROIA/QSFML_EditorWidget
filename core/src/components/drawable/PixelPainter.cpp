#include "components/drawable/PixelPainter.h"

namespace QSFML
{
    namespace Components
    {
        COMPONENT_IMPL(PixelPainter);

        PixelPainter::PixelPainter(const std::string& name)
            : Drawable(name)
            , m_position(0, 0)
            , m_pixelCount(0, 0)
            , m_pixelSize(1.0f)
            , m_pixels(nullptr)
            , m_texture()
            //, m_sprite(m_texture)
        {

        }
        PixelPainter::PixelPainter(const PixelPainter& other)
            : Drawable(other)
            , m_position(other.m_position)
            , m_pixelCount(other.m_pixelCount)
            , m_pixelSize(other.m_pixelSize)
            , m_pixels(new sf::Uint8[other.m_pixelCount.x * other.m_pixelCount.y * 4])         
        {
            memcpy(m_pixels, other.m_pixels, m_pixelCount.x * m_pixelCount.y * 4);
        }
        PixelPainter::~PixelPainter()
        {
            delete[] m_pixels;
        }

        void PixelPainter::setPixelSize(float size)
        {
            m_pixelSize = size;
        }
        float PixelPainter::getPixelSize() const
        {
            return m_pixelSize;
        }

        void PixelPainter::setPixelCount(const sf::Vector2u& count)
        {
            if(count == m_pixelCount)
				return;
            sf::Uint8 *newPixels = new sf::Uint8[count.x * count.y * 4];
            if (m_pixels)
            {
                for (unsigned int i = 0; i < count.y; ++i)
                {
                    for (unsigned int j = 0; j < count.x; ++j)
                    {
                        if (i < m_pixelCount.y && j < m_pixelCount.x)
                        {
							newPixels[(i * count.x + j) * 4 + 0] = m_pixels[(i * m_pixelCount.x + j) * 4 + 0];
							newPixels[(i * count.x + j) * 4 + 1] = m_pixels[(i * m_pixelCount.x + j) * 4 + 1];
							newPixels[(i * count.x + j) * 4 + 2] = m_pixels[(i * m_pixelCount.x + j) * 4 + 2];
							newPixels[(i * count.x + j) * 4 + 3] = m_pixels[(i * m_pixelCount.x + j) * 4 + 3];
						}
                        else
                        {
							newPixels[(i * count.x + j) * 4 + 0] = 0;
							newPixels[(i * count.x + j) * 4 + 1] = 0;
							newPixels[(i * count.x + j) * 4 + 2] = 0;
							newPixels[(i * count.x + j) * 4 + 3] = 0;
						}
					}
				}
				delete[] m_pixels;
			}
            m_pixelCount = count;
            m_texture.create(m_pixelCount.x, m_pixelCount.y);
            m_pixels = newPixels;
        }
        const sf::Vector2u& PixelPainter::getPixelCount() const
        {
            return m_pixelCount;
        }

        void PixelPainter::setPosition(const sf::Vector2f& pos)
        {
            m_position = pos;
        }
        const sf::Vector2f& PixelPainter::getPosition() const
        {
            return m_position;
        }

        void PixelPainter::setPixel(const sf::Vector2u& pos, const sf::Color& c)
        {
            if (pos.x < m_pixelCount.x && pos.y < m_pixelCount.y)
            {
                size_t index = (pos.y * m_pixelCount.x + pos.x) * 4;
				m_pixels[index + 0] = c.r;
				m_pixels[index + 1] = c.g;
				m_pixels[index + 2] = c.b;
				m_pixels[index + 3] = c.a;
			}
        }
        const sf::Color& PixelPainter::getPixel(const sf::Vector2u& pos) const
        {
            if (pos.x < m_pixelCount.x && pos.y < m_pixelCount.y)
            {
				size_t index = (pos.y * m_pixelCount.x + pos.x) * 4;
                return sf::Color(m_pixels[index + 0], 
                                 m_pixels[index + 1], 
                                 m_pixels[index + 2],
                                 m_pixels[index + 3]);
            }
            return sf::Color::Black;
        }

        void PixelPainter::clear()
        {
            memset(m_pixels, 0, m_pixelCount.x * m_pixelCount.y * 4);
        }

        void PixelPainter::draw(sf::RenderTarget& target,
            sf::RenderStates states) const
        {
            
			m_texture.update(m_pixels);
            sf::Sprite sprite;
            sprite.setTexture(m_texture);
            sprite.setPosition(m_position);
            sprite.setScale(m_pixelSize, m_pixelSize);
			target.draw(sprite, states);
        }
    }
}
