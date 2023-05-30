#include "components/KeyPressEvent.h"

namespace QSFML
{
namespace Components
{
COMPONENT_IMPL(KeyPressEvent)
KeyPressEvent::KeyPressEvent(const std::string &name,
                             sf::Keyboard::Key key)
    : AbstractPressEvent(name)
    , m_key(key)
{
 
}
KeyPressEvent::KeyPressEvent(const KeyPressEvent &other)
    : AbstractPressEvent(other)
    , m_key(other.m_key)
{

}
void KeyPressEvent::setTriggerKey(sf::Keyboard::Key key)
{
    m_key = key;
}
sf::Keyboard::Key KeyPressEvent::getTriggerKey() const
{
    return m_key;
}
bool KeyPressEvent::getCurrentValue()
{
    return sf::Keyboard::isKeyPressed(m_key);
}
}
}
