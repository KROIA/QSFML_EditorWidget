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
			loadFromImage(imagePath);
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

		bool Image::loadFromImage(const std::string& imagePath)
		{
			sf::Texture* t = new sf::Texture();
			if (t->loadFromFile(imagePath))
			{
				setTexture(t);
				return true;
			}
			delete t;
			return false;
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

		void Image::draw(sf::RenderTarget& target,
						 sf::RenderStates states) const
		{
			target.draw(*m_sprite);
		}





        void Image::setPosition(float x, float y)
		{
			m_sprite->setPosition(x, y);
		}
        void Image::setPosition(const sf::Vector2f& position)
		{
			m_sprite->setPosition(position);
		}
        void Image::setRotation(float angle)
		{
			m_sprite->setRotation(angle);
		}
        void Image::setScale(float factorX, float factorY)
		{
			m_sprite->setScale(factorX, factorY);
		}
        void Image::setScale(const sf::Vector2f& factors)
		{
			m_sprite->setScale(factors);
		}
        void Image::setOrigin(float x, float y)
		{
			m_sprite->setOrigin(x, y);
		}
        void Image::setOrigin(const sf::Vector2f& origin)
		{
			m_sprite->setOrigin(origin);
		}
        const sf::Vector2f& Image::getPosition() const
		{
			return m_sprite->getPosition();
		}
        float Image::getRotation() const
		{
			return m_sprite->getRotation();
		}
        const sf::Vector2f& Image::getScale() const
		{
			return m_sprite->getScale();
		}
        const sf::Vector2f& Image::getOrigin() const
		{
			return m_sprite->getOrigin();
		}
        void Image::move(float offsetX, float offsetY)
		{
			m_sprite->move(offsetX, offsetY);
		}
        void Image::move(const sf::Vector2f& offset)
		{
			m_sprite->move(offset);
		}
        void Image::rotate(float angle)
		{
			m_sprite->rotate(angle);
		}
        void Image::scale(float factorX, float factorY)
		{
			m_sprite->scale(factorX, factorY);
		}
        void Image::scale(const sf::Vector2f& factors)
		{
			m_sprite->scale(factors);
		}
        const sf::Transform& Image::getTransform() const
		{
			return m_sprite->getTransform();
		}
		const sf::Transform& Image::getInverseTransform() const
		{
			return m_sprite->getInverseTransform();
		}
	}
}