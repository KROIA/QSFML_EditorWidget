#include "components/MousePressEvent.h"
#include "objects/CanvasObject.h"
namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(MousePressEvent)
MousePressEvent::MousePressEvent(const std::string &name,
                                 sf::Mouse::Button button)
    : AbstractPressEvent(name)
    , m_button(button)
    , m_currentPressedWorldPosition(0,0)
    , m_currentPressedPixelPosition(0,0)
{

}
MousePressEvent::MousePressEvent(const MousePressEvent &other)
    : AbstractPressEvent(other)
    , m_button(other.m_button)
    , m_currentPressedWorldPosition(other.m_currentPressedWorldPosition)
    , m_currentPressedPixelPosition(other.m_currentPressedPixelPosition)
{

}
void MousePressEvent::setTriggerButton(sf::Mouse::Button button)
{
    m_button = button;
}
sf::Mouse::Button MousePressEvent::getTriggerButton() const
{
    return m_button;
}
const sf::Vector2f &MousePressEvent::getLastPressedWorldPos() const
{
    return m_currentPressedWorldPosition;
}
const sf::Vector2i &MousePressEvent::getLastPressedPixelPos() const
{
    return m_currentPressedPixelPosition;
}
bool MousePressEvent::getCurrentValue()
{
    bool isPressed = sf::Mouse::isButtonPressed(m_button);
    Objects::CanvasObject *parent = getParent();
    if(isPressed && parent)
    {
        m_currentPressedPixelPosition = parent->getMousePosition();
        m_currentPressedWorldPosition = parent->getMouseWorldPosition();
    }
    return isPressed;
}
}
}
