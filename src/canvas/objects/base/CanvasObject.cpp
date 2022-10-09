#include "CanvasObject.h"
#include "QSFML_Canvas.h"

CanvasObject::CanvasObject(CanvasObject *parent)
{
    m_canvasParent = nullptr;
    m_parent = parent;
}
CanvasObject::~CanvasObject()
{

}
void CanvasObject::setCanvasParent(QSFML_Canvas *parent)
{
    m_canvasParent = parent;
    internalOnCanvasParentChange(m_canvasParent);
    onCanvasParentChange(m_canvasParent);
}
QSFML_Canvas *CanvasObject::getCanvasParent() const
{
    return m_canvasParent;
}
void CanvasObject::setParent(CanvasObject *parent)
{
    m_parent = parent;
    internalOnParentChange(m_parent);
    onParentChange(m_parent);
}
CanvasObject *CanvasObject::getParent() const
{
    return m_parent;
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
sf::Vector2f CanvasObject::getInWorldSpace(const sf::Vector2i &pixelSpace)
{
    if(!m_canvasParent) return sf::Vector2f(0,0);
    return m_canvasParent->getInWorldSpace(pixelSpace);
}
sf::Vector2i CanvasObject::getInScreenSpace(const sf::Vector2f &worldSpace)
{
    if(!m_canvasParent) return sf::Vector2i(0,0);
    return m_canvasParent->getInScreenSpace(worldSpace);
}

void CanvasObject::addChild(CanvasObject *child)
{
    if(!child)return;
    if(childExists(child)) return;
    child->setParent(this);
    m_childs.push_back(child);
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
bool CanvasObject::childExists(CanvasObject *child)
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return true;
    return false;
}
size_t CanvasObject::getChildIndex(CanvasObject *child)
{
    for(size_t i=0; i<m_childs.size(); ++i)
        if(m_childs[i] == child)
            return i;
    return npos;
}
void CanvasObject::onCanvasParentChange(QSFML_Canvas *newParent) {}
void CanvasObject::onParentChange(CanvasObject *newParent) {}
//void CanvasObject::sfEvent(const sf::Event &e) {}
