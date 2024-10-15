#include "components/base/Drawable.h"
#include "objects/base/GameObject.h"
#include "Scene/Scene.h"

namespace QSFML
{
    namespace Components
    {
        Drawable::Drawable(const std::string& name)
            : Component(name)
            , sf::Transformable()
        {

        }
        Drawable::Drawable(const Drawable& other)
            : Component(other)
            , sf::Transformable(other)
        {

        }
        Drawable::~Drawable()
        {

        }

        sf::Vector2f Drawable::getGlobalPosition() const
        {
            if (m_parent)
                return m_parent->getGlobalPosition() + getPosition();
            return getPosition();
        }

        float Drawable::getGlobalRotation() const
        {
            if (m_parent)
                return m_parent->getGlobalRotation() + getRotation();
            return getRotation();
        }

        sf::Transform Drawable::getGlobalTransform() const
        {
			if (m_parent)
				return m_parent->getGlobalTransform() * getTransform();
			return getTransform();
        }

    }
}

