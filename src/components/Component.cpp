#include "Component.h"

using namespace QSFML::Components;
using namespace QSFML::Objects;

Component::Component(const std::string &name)
{
    m_name = name;
    setEnabled(true);
}
Component::Component(const Component &other)
{
    m_enabled = other.m_enabled;
    m_name = other.m_name;
    m_parent = nullptr;
}
Component::~Component()
{

}
Component *Component::clone() const
{
    return new Component(*this);
}

void Component::setParent(CanvasObject *parent)
{
    m_parent=parent;
}
CanvasObject *Component::getParent() const
{
    return m_parent;
}
void Component::setName(const std::string &name)
{
    m_name = name;
}
const std::string Component::getName() const
{
    return m_name;
}

void Component::setEnabled(bool enable)
{
    m_enabled = enable;
}
bool Component::isEnabled() const
{
    return m_enabled;
}
void Component::update()
{

}
