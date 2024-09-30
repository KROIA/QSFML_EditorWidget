#include "objects/base/GameObject.h"

namespace QSFML
{
	namespace Objects
	{
        void GameObject::setPosition(float x, float y)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setPosition(x, y);
        }
        void GameObject::setPosition(const sf::Vector2f& position)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setPosition(position);
        }
        void GameObject::setRotation(float angle)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setRotation(angle);
        }
        void GameObject::setScale(float factorX, float factorY)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setScale(factorX, factorY);
        }
        void GameObject::setScale(const sf::Vector2f& factors)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setScale(factors);
        }
        void GameObject::setOrigin(float x, float y)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setOrigin(x, y);
        }
        void GameObject::setOrigin(const sf::Vector2f& origin)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->setOrigin(origin);
        }
        const sf::Vector2f& GameObject::getPosition() const
        {
            if (!m_componentsManagerData.transform)
            {
                static sf::Vector2f zero(0, 0);
                return zero;
            }
            return m_componentsManagerData.transform->getPosition();
        }
        sf::Vector2f GameObject::getGlobalPosition() const
        {
            sf::Transform globalTransform = getGlobalTransform();
            return globalTransform.transformPoint({0.0, 0.0});
        }
        float GameObject::getRotation() const
        {
            if (!m_componentsManagerData.transform) return 0;
            return m_componentsManagerData.transform->getRotation();
        }
        float GameObject::getGlobalRotation() const
        {
            sf::Transform globalTransform = getGlobalTransform();
            // Get the rotation from the global transform
            return std::atan2(globalTransform.getMatrix()[1], globalTransform.getMatrix()[0]) * 180 / 3.14159265;
        }
        const sf::Vector2f& GameObject::getScale() const
        {
            if (!m_componentsManagerData.transform)
            {
                static sf::Vector2f one(1, 1);
				return one;
            }
            return m_componentsManagerData.transform->getScale();
        }
        sf::Vector2f GameObject::getGlobalScale() const
        {
            sf::Transform globalTransform = getGlobalTransform();
			// Get the scale from the global transform
            return sf::Vector2f(
                std::sqrt(globalTransform.getMatrix()[0] * globalTransform.getMatrix()[0] + globalTransform.getMatrix()[1] * globalTransform.getMatrix()[1]),
                std::sqrt(globalTransform.getMatrix()[4] * globalTransform.getMatrix()[4] + globalTransform.getMatrix()[5] * globalTransform.getMatrix()[5]));

        }
        const sf::Vector2f& GameObject::getOrigin() const
        {
            if (!m_componentsManagerData.transform)
            {
                static sf::Vector2f zero(0, 0);
				return zero;
            }
            return m_componentsManagerData.transform->getOrigin();
        }
        void GameObject::move(float offsetX, float offsetY)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->move(offsetX, offsetY);
        }
        void GameObject::move(const sf::Vector2f& offset)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->move(offset);
        }
        void GameObject::rotate(float angle)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->rotate(angle);
        }
        void GameObject::scale(float factorX, float factorY)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->scale(factorX, factorY);
        }
        void GameObject::scale(const sf::Vector2f& factor)
        {
            if (!m_componentsManagerData.transform) return;
            m_componentsManagerData.transform->scale(factor);
        }
        const sf::Transform& GameObject::getTransform() const
        {
            if (!m_componentsManagerData.transform)
            {
                static sf::Transform identity;
				return identity;
            }
            return m_componentsManagerData.transform->getTransform();
        }
        const sf::Transform& GameObject::getInverseTransform() const
        {
            if (!m_componentsManagerData.transform)
            {
                static sf::Transform identity;
                return identity;
            }
            return m_componentsManagerData.transform->getInverseTransform();
        }

        sf::Transform GameObject::getGlobalTransform() const
        {
            QSFMLP_OBJECT_FUNCTION(QSFML_COLOR_STAGE_1);
            std::vector<GameObjectPtr> parents;
            parents.reserve(10);
            GameObjectPtr parent = m_parent;
            while (parent)
			{
				parents.push_back(parent);
				parent = parent->m_parent;
			}

            sf::Transform globalTransform;
            for (auto it = parents.rbegin(); it != parents.rend(); ++it)
			{
				globalTransform = (*it)->getTransform() * globalTransform;
			}
            globalTransform = getTransform() * globalTransform;
            return globalTransform;
        }
	}
}