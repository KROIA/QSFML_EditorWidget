#include "components/drawable/Image.h"
#include <math.h>
#include <SFML/Graphics.hpp>

namespace QSFML
{
	namespace Components
	{
		COMPONENT_IMPL(Image);

		Image::Image(const std::string& name)
			: Drawable(name)
			, m_sprite(nullptr)
			, m_texture(nullptr)
			, m_selfOwnedTexture(nullptr)
		{
            m_sprite = new sf::Sprite();
		}
		Image::Image(const sf::Texture* t, const std::string& name) 
			: Drawable(name)
			, m_sprite(nullptr)
			, m_texture(nullptr)
			, m_selfOwnedTexture(nullptr)
		{
			setTexture(t);
            if (!m_sprite)
                m_sprite = new sf::Sprite();
		}
		Image::Image(const std::string& imagePath, const std::string& name)
			: Drawable(name)
			, m_sprite(nullptr)
			, m_texture(nullptr)
			, m_selfOwnedTexture(nullptr)
		{
			loadFromFile(imagePath);
            if (!m_sprite)
                m_sprite = new sf::Sprite();
		}

		Image::Image(const Image& other)
			: Drawable(other)
			, m_sprite(nullptr)
			, m_texture(nullptr)
			, m_selfOwnedTexture(nullptr)
		{
			if (other.m_selfOwnedTexture)
			{
				m_selfOwnedTexture = new sf::Texture(*other.m_selfOwnedTexture);
			}
			m_texture = m_selfOwnedTexture;
			createSprite();
            if (!m_sprite)
                m_sprite = new sf::Sprite();
		}

        Image::~Image()
        {
            delete m_sprite;
            delete m_selfOwnedTexture;
        }


		void Image::setTexture(const sf::Texture* t)
		{
			if(m_selfOwnedTexture)
			{
				delete m_selfOwnedTexture;
				m_selfOwnedTexture = nullptr;
			}
			m_texture = t;
			createSprite();
		}
		const sf::Texture* Image::getTexture() const
		{
			return m_texture;
		}

		bool Image::loadFromFile(const std::string& imagePath)
		{
			sf::Texture* t = new sf::Texture();
			if (t->loadFromFile(imagePath))
			{
				setTexture(t);
				m_selfOwnedTexture = t;
				return true;
			}
			delete t;
			return false;
		}
		void Image::loadFromImage(const sf::Image& image)
		{
			sf::Texture* t = new sf::Texture();
			t->loadFromImage(image);
			setTexture(t);
			m_selfOwnedTexture = t;
		}

		void Image::createSprite()
		{
			if (!m_texture)
				return;
			if (!m_sprite)
				m_sprite = new sf::Sprite(*m_texture);
			else
				m_sprite->setTexture(*m_texture);
		}

		void Image::drawComponent(sf::RenderTarget& target,
						 sf::RenderStates states) const
		{
			target.draw(*m_sprite, states);
		}





       
	}
}
