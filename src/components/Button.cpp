#include "Button.h"
#include "CanvasObject.h"
namespace QSFML
{
namespace Components
{
Button::Button(const std::string &name)
    :   MousePressEvent(name)
    ,   Utilities::AABB()
{
}
Button::Button(const std::string &name, const Utilities::AABB &box)
    :   MousePressEvent(name)
    ,   Utilities::AABB(box)
{

}
Button::~Button()
{

}

bool Button::getCurrentValue()
{
    bool isPressed = sf::Mouse::isButtonPressed(getTriggerButton());
    if(!isPressed)
        return false;

    sf::Vector2f pos = getParent()->getMouseWorldPosition();
    if(contains(pos))
        return true;
    return false;
}
}
}
