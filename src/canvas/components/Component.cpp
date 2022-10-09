#include "Component.h"


Component::Component(const std::string &name)
{
    m_name = name;
}
Component::~Component()
{

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
