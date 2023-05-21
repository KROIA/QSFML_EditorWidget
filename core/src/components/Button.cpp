#include "components/Button.h"
#include "objects/CanvasObject.h"
namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(Button);
Button::Button(const std::string &name)
    : MousePressEvent(name)
    , Utilities::AABB()
    , m_wasInside(false)
    , m_wasPressed(false)
{

}
Button::Button(const std::string &name, const Utilities::AABB &box)
    : MousePressEvent(name)
    , Utilities::AABB(box)
    , m_wasInside(false)
    , m_wasPressed(false)
{

}
Button::Button(const Button &other)
    : MousePressEvent(other)
    , Utilities::AABB(other)
    , m_wasInside(other.m_wasInside)
    , m_wasPressed(other.m_wasPressed)
{

}
Button::~Button()
{

}

bool Button::getCurrentValue()
{
    bool isPressed = sf::Mouse::isButtonPressed(getTriggerButton());
    sf::Vector2f pos = getParent()->getMouseWorldPosition();
    bool isInside = contains(pos);

    if(isPressed && m_wasInside && isInside)
    {
        m_wasInside = isInside;
        m_wasPressed = isPressed;
        return true;
    }

    m_wasInside = isInside;
    m_wasPressed = isPressed;

    return false;
}
}
}
