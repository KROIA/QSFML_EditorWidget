#include "components/Component.h"
#include "objects/CanvasObject.h"

using namespace QSFML::Components;
using namespace QSFML::Objects;

COMPONENT_IMPL(Component)

Component::Component(const std::string &name)
    : m_enabled(true)
    , m_name(name)
    , m_parent(nullptr)
{

}
Component::Component(const Component &other)
    : m_enabled(other.m_enabled)
    , m_name(other.m_name)
    , m_parent(nullptr)
{

}
Component::~Component()
{
    if (m_parent)
    {
        CanvasObject* parent = m_parent;
        m_parent = nullptr;
        parent->removeComponent(this);
    }
}



void Component::setParent(CanvasObject *parent)
{
    m_parent = parent;
}
QSFML::Objects::CanvasObject* Component::getParent() const
{ 
    return m_parent; 
}
QSFML::Canvas* Component::getCanvasParent() const
{
    if (m_parent)
        return m_parent->getCanvasParent();
    return nullptr;
}
void Component::setName(const std::string &name)
{
    m_name = name;
}


void Component::setEnabled(bool enable)
{
    m_enabled = enable;
}

void Component::deleteThis()
{
    if (m_parent)
    {
        CanvasObject* parent = m_parent;
        m_parent = nullptr;
        parent->deleteComponent(this);
    }
}
