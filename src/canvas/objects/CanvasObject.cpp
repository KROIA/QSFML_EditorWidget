#include "CanvasObject.h"
#include "Canvas.h"
#include "Drawable.h"
#include "SfEventHandle.h"


using namespace QSFML;
using namespace QSFML::Objects;
using namespace QSFML::Components;

size_t CanvasObject::m_objNameCounter = 0;
CanvasObject::CanvasObject(const std::string &name, CanvasObject *parent)
{
    m_name = name;
    if(m_name.size() == 0)
    {
        m_objNameCounter++;
        m_name = "CanvasObject_"+std::to_string(m_objNameCounter);
    }
    m_canvasParent = nullptr;
    m_parent = parent;
    setEnabled(true);
}
CanvasObject::~CanvasObject()
{
    for(size_t i=0; i<m_childs.size(); ++i)
        delete m_childs[i];
    m_childs.clear();

    for(size_t i=0; i<m_components.size(); ++i)
        delete m_components[i];
    m_components.clear();
    if(m_canvasParent)
        m_canvasParent->removeObject(this);
}


void CanvasObject::setParent(CanvasObject *parent)
{
    if(m_parent)
    {
        size_t index = m_parent->getChildIndex(this);
        if(index == npos) return;
        m_parent->m_childs.erase(m_parent->m_childs.begin() + index);
    }
    m_parent = parent;
    for(size_t i=0; i<m_components.size(); ++i)
        m_components[i]->setParent(this);
    internalOnParentChange(m_parent);
    onParentChange(m_parent);
}
void CanvasObject::setEnabled(bool enable)
{
    m_enabled = enable;
}
bool CanvasObject::isEnabled() const
{
    return m_enabled;
}
CanvasObject *CanvasObject::getParent() const
{
    return m_parent;
}

void CanvasObject::setName(const std::string &name)
{
    m_name = name;
}
const std::string CanvasObject::getName() const
{
    return m_name;
}

void CanvasObject::addChild(CanvasObject *child)
{
    if(!child)return;
    if(childExists(child)) return;
    child->setParent(this);
    m_childs.push_back(child);
}


sf::Vector2i CanvasObject::getMousePosition() const
{
    if(!m_canvasParent) return sf::Vector2i(0,0);
    return m_canvasParent->getMousePosition();
}
sf::Vector2f CanvasObject::getMouseWorldPosition() const
{
    if(!m_canvasParent) return sf::Vector2f(0,0);
    return m_canvasParent->getMouseWorldPosition();
}
sf::Vector2f CanvasObject::getInWorldSpace(const sf::Vector2i &pixelSpace) const
{
    if(!m_canvasParent) return sf::Vector2f(0,0);
    return m_canvasParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i CanvasObject::getInScreenSpace(const sf::Vector2f &worldSpace) const
{
    if(!m_canvasParent) return sf::Vector2i(0,0);
    return m_canvasParent->getInScreenSpace(worldSpace);
}


void CanvasObject::removeChild(CanvasObject *child)
{
    if(!child)return;
    size_t index = getChildIndex(child);
    if(index == npos) return;
    child->setParent(nullptr);
    m_childs.erase(m_childs.begin() + index);
}
void CanvasObject::deleteChilds()
{
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        m_childs[i]->setParent(nullptr);
        delete m_childs[i];
    }
    m_childs.clear();
}
bool CanvasObject::childExists(CanvasObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return true;
    return false;
}
size_t CanvasObject::getChildIndex(CanvasObject *child) const
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return i;
    return npos;
}
const std::vector<CanvasObject*> &CanvasObject::getChilds() const
{
    return m_childs;
}
size_t CanvasObject::getChildCount() const
{
    return m_childs.size();
}


void CanvasObject::addComponent(Component *comp)
{
    if(!comp)return;
    if(componentExists(comp)) return;
    comp->setParent(this);
    m_components.push_back(comp);
}
void CanvasObject::removeComponent(Component *comp)
{
    if(!comp)return;
    size_t index = getComponentIndex(comp);
    if(index == npos) return;
    comp->setParent(nullptr);
    m_components.erase(m_components.begin() + index);
}
void CanvasObject::deleteComponents()
{
    for(size_t i=0; i<m_components.size(); ++i)
    {
        m_components[i]->setParent(nullptr);
        delete m_components[i];
    }
    m_components.clear();
}
bool CanvasObject::componentExists(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return true;
    return false;
}
size_t CanvasObject::getComponentIndex(Component *comp) const
{
    for(size_t i=0; i<m_components.size(); ++i)
        if(m_components[i] == comp)
            return i;
    return npos;
}
const std::vector<Component*> &CanvasObject::getComponents() const
{
    return m_components;
}

size_t CanvasObject::getComponentCount() const
{
    return m_components.size();
}

const sf::View CanvasObject::getCameraView() const
{
    static const sf::View dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getCameraView();
}
const sf::View &CanvasObject::getDefaultCameraView() const
{
    static const sf::View dummy;
    if(!m_canvasParent) return dummy;
    return m_canvasParent->getDefaultCameraView();
}
sf::Vector2u CanvasObject::getCanvasSize() const
{
    if(!m_canvasParent) return sf::Vector2u(0,0);
    return m_canvasParent->getCanvasSize();
}
sf::Vector2u CanvasObject::getOldCanvasSize() const
{
    if(!m_canvasParent) return sf::Vector2u(0,0);
    return m_canvasParent->getOldCanvasSize();
}




std::string CanvasObject::toString() const
{
    using std::string;
    std::vector<string> lines = toStringInternal("");
    string msg;
    for(size_t i=0; i<lines.size(); ++i)
    {
        msg += lines[i]+"\n";
    }

    return msg;
}
std::vector<std::string> CanvasObject::toStringInternal(const std::string &preStr) const
{
    using std::string;
    std::vector<string> lines;
    const CanvasObject *t = this;

    auto en = [](bool enabled) {
        return enabled?"[Enabled ]":"[Disabled]";
      };

    lines.push_back(preStr+" | "+string(typeid(t).name())+": \""+m_name + "\" "+en(t->isEnabled()));
    lines.push_back(preStr+" |  Components:");
    for(size_t i=0; i<m_components.size(); ++i)
    {
        lines.push_back(preStr+  " |  - " +string(typeid(m_components[i]).name())+": \""+m_components[i]->getName()+ "\" "+en(m_components[i]->isEnabled()));
    }
    lines.push_back(preStr+" |  Childs:");
    for(size_t i=0; i<m_childs.size(); ++i)
    {

        std::vector<std::string> subList = m_childs[i]->toStringInternal(preStr+" | ");
        for(size_t j=0; j<subList.size(); ++j)
        {
            lines.push_back(subList[j]);
        }

    }
    lines.push_back(preStr+" ----");
    return lines;
}

void CanvasObject::onCanvasParentChange(Canvas *newParent) {}
void CanvasObject::onParentChange(CanvasObject *newParent) {}
void CanvasObject::internalOnCanvasParentChange(Canvas *newParent) {}
void CanvasObject::internalOnParentChange(CanvasObject *newParent) {}

Canvas *CanvasObject::getCanvasParent() const
{
    return m_canvasParent;
}

void CanvasObject::setCanvasParent(Canvas *parent)
{
    m_canvasParent = parent;
    for(size_t i=0; i<m_components.size(); ++i)
        m_components[i]->setParent(this);

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->setCanvasParent(parent);

    internalOnCanvasParentChange(m_canvasParent);
    onCanvasParentChange(m_canvasParent);
}
void CanvasObject::sfEvent(const std::vector<sf::Event> &events)
{
    if(!m_enabled) return;
    for(size_t i=0; i<m_components.size(); ++i)
    {
        if(!m_components[i]->isEnabled())
            continue;
        SfEventHandle* comp = dynamic_cast<SfEventHandle*>(m_components[i]);
        if(comp)
        {
            for(size_t j=0; j<events.size(); ++j)
                comp->sfEvent(events[j]);
        }
    }
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->sfEvent(events);
    }
}
void CanvasObject::draw(sf::RenderWindow &window) const
{
    if(!m_enabled) return;
    for(size_t i=0; i<m_components.size(); ++i)
    {
        if(!m_components[i]->isEnabled())
            continue;
        Drawable* comp = dynamic_cast<Drawable*>(m_components[i]);
        if(comp)
        {
            window.draw(*comp);
        }
    }
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        if(m_childs[i]->m_enabled)
            m_childs[i]->draw(window);
    }
}

