#include "CanvasObject.h"
#include "QSFML_Canvas.h"
#include "DrawableComponent.h"
#include "SfEventHandleComponent.h"

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
}
CanvasObject::~CanvasObject()
{
    for(size_t i=0; i<m_childs.size(); ++i)
        delete m_childs[i];
    m_childs.clear();

    for(size_t i=0; i<m_components.size(); ++i)
        delete m_components[i];
    m_components.clear();
}

void CanvasObject::setCanvasParent(QSFML_Canvas *parent)
{
    m_canvasParent = parent;
    for(size_t i=0; i<m_components.size(); ++i)
        m_components[i]->setParent(this);

    for(size_t i=0; i<m_childs.size(); ++i)
        m_childs[i]->setCanvasParent(parent);

    internalOnCanvasParentChange(m_canvasParent);
    onCanvasParentChange(m_canvasParent);
}
QSFML_Canvas *CanvasObject::getCanvasParent() const
{
    return m_canvasParent;
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
void CanvasObject::clearChilds()
{
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        m_childs[i]->setParent(nullptr);
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
void CanvasObject::clearComponents()
{
    for(size_t i=0; i<m_components.size(); ++i)
    {
        m_components[i]->setParent(nullptr);
        //delete m_components[i];
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
template<typename T>
std::vector<T*> CanvasObject::getComponents() const
{
    std::vector<T*> list;
    list.reserve(m_components.size());
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(comp)
            list.push_back(comp);
    }
    return list;
}
size_t CanvasObject::getComponentCount() const
{
    return m_components.size();
}
template<typename T>
size_t CanvasObject::getComponentCount() const
{
    size_t count = 0;
    for(size_t i=0; i<m_components.size(); ++i)
    {
        T* comp = dynamic_cast<T*>(m_components[i]);
        if(comp)
            ++count;
    }
    return count;
}

void CanvasObject::sfEvent(const std::vector<sf::Event> &events)
{
    for(size_t i=0; i<m_components.size(); ++i)
    {
        SfEventHandleComponent* comp = dynamic_cast<SfEventHandleComponent*>(m_components[i]);
        if(comp)
        {
            for(size_t j=0; j<events.size(); ++j)
                comp->sfEvent(events[j]);
        }
    }
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        m_childs[i]->sfEvent(events);
    }
}
void CanvasObject::draw(sf::RenderWindow &window) const
{
    for(size_t i=0; i<m_components.size(); ++i)
    {
        DrawableComponent* comp = dynamic_cast<DrawableComponent*>(m_components[i]);
        if(comp)
        {
            window.draw(*comp);
        }
    }
    for(size_t i=0; i<m_childs.size(); ++i)
    {
        m_childs[i]->draw(window);
    }
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

    lines.push_back(preStr+" | "+string(typeid(t).name())+": "+m_name);
    lines.push_back(preStr+" |  Components:");
    for(size_t i=0; i<m_components.size(); ++i)
    {
        lines.push_back(preStr+  " |  - " +string(typeid(m_components[i]).name())+": "+m_components[i]->getName());
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

void CanvasObject::onCanvasParentChange(QSFML_Canvas *newParent) {}
void CanvasObject::onParentChange(CanvasObject *newParent) {}
//void CanvasObject::sfEvent(const sf::Event &e) {}

