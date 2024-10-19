#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics.hpp>

namespace QSFML
{
	namespace Utilities
	{
		class QSFML_EDITOR_WIDGET_EXPORT Transformable
		{
		public:
        Transformable()
        {

        }
        Transformable(const sf::Vector2f& pos, float rot)
        {
			setRotation(rot);
			setPosition(pos);
        }

        bool operator==(const Transformable& other) const
        {
            return m_transformable.getTransform() == other.m_transformable.getTransform();
        }
		bool operator!=(const Transformable& other) const
		{
			return m_transformable.getTransform() != other.m_transformable.getTransform();
		}

        void setPosition(float x, float y)
        {
			m_transformable.setPosition(x, y);
        }
        void setPosition(const sf::Vector2f& position)
        {
			m_transformable.setPosition(position);
        }
        void setRotation(float angle)
        {
			m_transformable.setRotation(angle);
        }
        void setScale(float factorX, float factorY)
        {
			m_transformable.setScale(factorX, factorY);
        }
        void setScale(const sf::Vector2f& factors)
        {
			m_transformable.setScale(factors);
        }
        void setOrigin(float x, float y)
        {
			m_transformable.setOrigin(x, y);
        }
        void setOrigin(const sf::Vector2f& origin)
        {
			m_transformable.setOrigin(origin);
        }
        const sf::Vector2f& getPosition() const
        {
			return m_transformable.getPosition();
        }
        float getRotation() const
        {
			return m_transformable.getRotation();
        }
        const sf::Vector2f& getScale() const
        {
			return m_transformable.getScale();
        }
        const sf::Vector2f& getOrigin() const
        {
			return m_transformable.getOrigin();
        }
        void move(float offsetX, float offsetY)
        {
			m_transformable.move(offsetX, offsetY);
        }
        void move(const sf::Vector2f& offset)
        {
			m_transformable.move(offset);
        }
        void rotate(float angle)
        {
			m_transformable.rotate(angle);
        }
        void scale(float factorX, float factorY)
        {
			m_transformable.scale(factorX, factorY);
        }
        void scale(const sf::Vector2f& factor)
        {
			m_transformable.scale(factor);
        }
        const sf::Transform& getTransform() const
        {
			return m_transformable.getTransform();
        }
        const sf::Transform& getInverseTransform() const
        {
			return m_transformable.getInverseTransform();
        }

        const sf::Transform& getGlobalTransform() const
        {
            // <! ToDo only update if needed
            updateGlobalTransform();
            return m_globalTransform;
        }

			
		protected:
        void markDirty()
        {
			m_needsTransformUpdate = true;
        }
        void updateGlobalTransform() const
        {
            Transformable* parent = m_transformParent;
			QSFML::vector<const Transformable*> transformTree;
            transformTree.reserve(20);
			while (parent)
			{
				transformTree.push_back(parent);
				parent = parent->m_transformParent;
			}

			sf::Transform globalTransform;
            for (auto it = transformTree.rbegin(); it != transformTree.rend(); ++it)
            {
                globalTransform.combine((*it)->m_transformable.getTransform());
            }
			globalTransform.combine(m_transformable.getTransform());
			m_globalTransform = globalTransform;
			m_needsTransformUpdate = false;
        }

		private:


			Transformable* m_transformParent = nullptr;
			//QSFML::vector<Transformable*> m_children;

			sf::Transformable m_transformable;

			mutable bool m_needsTransformUpdate = false;
			mutable sf::Transform m_globalTransform;
		};
	}
}