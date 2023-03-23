#include "components/Component.h"

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
void Component::setName(const std::string &name)
{
    m_name = name;
}


void Component::setEnabled(bool enable)
{
    m_enabled = enable;
}
