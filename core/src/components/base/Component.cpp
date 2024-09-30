#include "components/base/Component.h"
#include "objects/base/GameObject.h"

using namespace QSFML::Components;
using namespace QSFML::Objects;

COMPONENT_IMPL(Component)

Component::Component(const std::string &name)
    : m_enabled(true)
    , m_name(name)
    , m_parent(nullptr)
    , m_SceneParent(nullptr)
{

}
Component::Component(const Component &other)
    : m_enabled(other.m_enabled)
    , m_name(other.m_name)
    , m_parent(nullptr)
    , m_SceneParent(nullptr)
{

}
Component::~Component()
{
    if (m_parent)
    {
        GameObject* parent = m_parent;
        m_parent = nullptr;
        parent->removeComponent(this);
    }
}









void Component::deleteThis()
{
    if (m_parent)
    {
        GameObject* parent = m_parent;
        m_parent = nullptr;
        parent->deleteComponent(this);
    }
}



sf::Vector2i Component::getMousePosition() const
{
    if (!m_parent) return sf::Vector2i(0, 0);
    return m_parent->getMousePosition();
}
sf::Vector2f Component::getMouseWorldPosition() const
{
    if (!m_parent) return sf::Vector2f(0, 0);
    return m_parent->getMouseWorldPosition();
}
sf::Vector2f Component::getInWorldSpace(const sf::Vector2i& pixelSpace) const
{
    if (!m_parent) return sf::Vector2f(0, 0);
    return m_parent->getInWorldSpace(pixelSpace);
}
sf::Vector2i Component::getInScreenSpace(const sf::Vector2f& worldSpace) const
{
    if (!m_parent) return sf::Vector2i(0, 0);
    return m_parent->getInScreenSpace(worldSpace);
}
const sf::View Component::getCameraView() const
{
    static const sf::View dummy;
    if (!m_parent) return dummy;
    return m_parent->getCameraView();
}
const sf::View& Component::getDefaultCameraView() const
{
    static const sf::View dummy;
    if (!m_parent) return dummy;
    return m_parent->getDefaultCameraView();
}
QSFML::Utilities::AABB Component::getCameraViewRect() const
{
    if (!m_parent) return Utilities::AABB();
    return m_parent->getCameraViewRect();
}
sf::Vector2u Component::getSceneSize() const
{
    if (!m_parent) return sf::Vector2u(0, 0);
    return m_parent->getSceneSize();
}
sf::Vector2u Component::getOldSceneSize() const
{
    if (!m_parent) return sf::Vector2u(0, 0);
    return m_parent->getOldSceneSize();
}

const sf::Font& Component::getTextFont() const
{
    const static sf::Font dummy;
    if (!m_parent) return dummy;
    return m_parent->getDefaultTextFont();
}
size_t Component::getTick() const
{
    if (!m_parent) return 0;
    return m_parent->getTick();
}
double Component::getDeltaT() const
{
    if (!m_parent) return 0;
    return m_parent->getDeltaT();
}

QSFML::Scene* Component::getSceneParent() const
{
    return m_SceneParent;
}
