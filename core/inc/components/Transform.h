#pragma once

#include "QSFML_EditorWidget_base.h"
#include <SFML/Graphics.hpp>
#include "components/base/Component.h"

namespace QSFML
{
	namespace Components
	{
		class QSFML_EDITOR_WIDGET_EXPORT Transform : public Component
		{
		public:
        COMPONENT_IMPL(Transform);
        Transform()
			: Component("Transform")
        {

        }
        Transform(const sf::Vector2f& pos, float rot)
            : Component("Transform")
        {
			setRotation(rot);
			setPosition(pos);
        }
        Transform(const Transform& other)
            : m_transformable(other.m_transformable)
            , m_needsTransformUpdate(other.m_needsTransformUpdate)
        {
            m_globalTransform = other.m_globalTransform;
        }

        bool operator==(const Transform& other) const
        {
            return m_transformable.getTransform() == other.m_transformable.getTransform();
        }
		bool operator!=(const Transform& other) const
		{
			return m_transformable.getTransform() != other.m_transformable.getTransform();
		}

        void setPosition(float x, float y)
        {
			m_transformable.setPosition(x, y);
            markDirty();
        }
        void setPosition(const sf::Vector2f& position)
        {
			m_transformable.setPosition(position);
            markDirty();
        }
        void setRotation(float angle)
        {
			m_transformable.setRotation(angle);
            markDirty();
        }
        void setScale(float factorX, float factorY)
        {
			m_transformable.setScale(factorX, factorY);
            markDirty();
        }
        void setScale(const sf::Vector2f& factors)
        {
			m_transformable.setScale(factors);
            markDirty();
        }
        void setOrigin(float x, float y)
        {
			m_transformable.setOrigin(x, y);
            markDirty();
        }
        void setOrigin(const sf::Vector2f& origin)
        {
			m_transformable.setOrigin(origin);
            markDirty();
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
            markDirty();
        }
        void move(const sf::Vector2f& offset)
        {
			m_transformable.move(offset);
            markDirty();
        }
        void rotate(float angle)
        {
			m_transformable.rotate(angle);
            markDirty();
        }
        void scale(float factorX, float factorY)
        {
			m_transformable.scale(factorX, factorY);
            markDirty();
        }
        void scale(const sf::Vector2f& factor)
        {
			m_transformable.scale(factor);
            markDirty();
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
            return m_globalTransform;
        }

		bool isDirty() const
		{
			return m_needsTransformUpdate;
		}
        void markDirty()
        {
            m_needsTransformUpdate = true;
        }
        void markUndirty() const
        {
            m_needsTransformUpdate = false;
        }

		void setParentTransform(const sf::Transform& parentTransform)
		{
            m_globalTransform = parentTransform * getTransform();
			markUndirty();
		}

        
        //const sf::Transform& getGlobalTransform() const
        //{
        //    // <! ToDo only update if needed
        //    updateGlobalTransform();
        //    return m_globalTransform;
        //}

			
		protected:
        
        void updateGlobalTransform() const
        {
           /* Transformable* parent = m_transformParent;
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
			m_needsTransformUpdate = false;*/
        }

		private:


			//Transformable* m_transformParent = nullptr;
			//QSFML::vector<Transformable*> m_children;

			sf::Transformable m_transformable;

			mutable bool m_needsTransformUpdate = false;
			mutable sf::Transform m_globalTransform;
		};

        
	}
}