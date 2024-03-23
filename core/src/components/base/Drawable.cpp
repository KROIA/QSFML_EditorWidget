#include "components/base/Drawable.h"
#include "objects/base/CanvasObject.h"
#include "canvas/Canvas.h"

namespace QSFML
{
    namespace Components
    {
        Drawable::Drawable(const std::string& name)
            : Component(name)
            , Utilities::Transformable()
        {

        }
        Drawable::Drawable(const Drawable& other)
            : Component(other)
            , Utilities::Transformable()
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

    }
}

