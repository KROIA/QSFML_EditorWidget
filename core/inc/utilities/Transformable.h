#pragma once

#include "QSFML_EditorWidget_base.h"

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT Transformable
		{
		public:
			Transformable()
				: m_position(0, 0)
				, m_rotation(0)
			{	}
			Transformable(const Transformable &other)
				: m_position(other.m_position)
				, m_rotation(other.m_rotation)
			{	}
			Transformable(const sf::Vector2f& position, float rotation)
				: m_position(position)
				, m_rotation(rotation)
			{	}
			virtual ~Transformable()
			{	}

			Transformable& operator=(const Transformable &other)
			{
				m_position = other.m_position;
				m_rotation = other.m_rotation;
				return *this;
			}

			bool operator==(const Transformable &other) const
			{
				return m_position == other.m_position && m_rotation == other.m_rotation;
			}
		bool operator!=(const Transformable &other) const
			{
				return !(*this == other);
			}

			void setPosition(const sf::Vector2f& position)
			{
				sf::Vector2f oldPosition = m_position;
				m_position = position;
				positionChanged(oldPosition, m_position);
			}
			void setPosition(float x, float y)
			{
				sf::Vector2f oldPosition = m_position;
				m_position.x = x;
				m_position.y = y;
				positionChanged(oldPosition, m_position);
			}
			const sf::Vector2f& getPosition() const
			{
				return m_position;
			}

			void setRotation(float rotation)
			{
				float oldRotation = m_rotation;
				m_rotation = rotation;
				rotationChanged(oldRotation, m_rotation);
			}
			float getRotation() const
			{
				return m_rotation;
			}

			void move(const sf::Vector2f& offset)
			{
				sf::Vector2f oldPosition = m_position;
				m_position += offset;
				positionChanged(oldPosition, m_position);
			}
			void move(float offsetX, float offsetY)
			{
				sf::Vector2f oldPosition = m_position;
				m_position.x += offsetX;
				m_position.y += offsetY;
				positionChanged(oldPosition, m_position);
			}

			void rotate(float angle)
			{
				float oldRotation = m_rotation;
				m_rotation += angle;
				rotationChanged(oldRotation, m_rotation);
			}


			
		protected:
			virtual void positionChanged(const sf::Vector2f &oldPosition, const sf::Vector2f &newPosition) = 0;
			virtual void rotationChanged(float oldRotation, float newRotation) = 0;

		private:
			sf::Vector2f m_position;
			float m_rotation;
		};
	}
}