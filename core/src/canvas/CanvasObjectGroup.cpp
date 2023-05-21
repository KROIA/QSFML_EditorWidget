#include "canvas/CanvasObjectGroup.h"
#include "canvas/Canvas.h"


using namespace QSFML::Objects;
using namespace QSFML::Components;
using namespace QSFML;

CanvasObjectGroup::CanvasObjectGroup(Canvas *parent)
{
    m_parent = parent;
}
CanvasObjectGroup::~CanvasObjectGroup()
{
    for(size_t i=0; i<m_toAddContainer.size(); ++i)
    {
        delete m_toAddContainer[i];
    }
    m_toAddContainer.clear();
    for(size_t i=0; i<m_container.size(); ++i)
    {
        delete m_container[i];
    }
    m_container.clear();
}

void CanvasObjectGroup::addObject(CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);

    if(obj->getCanvasParent() != m_parent && obj->getCanvasParent())
        obj->getCanvasParent()->removeObject(obj);
    m_toAddContainer.push_back(obj);
    obj->setCanvasParent(m_parent);
}
void CanvasObjectGroup::addObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
    for(size_t i=0; i<objs.size(); ++i)
    {
        addObject(objs[i]);
    }
}
void CanvasObjectGroup::addObject_internal()
{
    for(size_t i=0; i<m_toAddContainer.size(); ++i)
    {
        if(objectExists(m_toAddContainer[i]))
            continue;
        m_container.push_back(m_toAddContainer[i]);
    }
    m_toAddContainer.clear();
}
void CanvasObjectGroup::deleteObject_internal()
{
    for(size_t i=0; i<m_toDelete.size(); ++i)
    {
        deleteObject(m_toDelete[i]);
    }
    m_toDelete.clear();
}

void CanvasObjectGroup::removeObject(CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
    size_t index = getObjectIndex(obj);
    if(index == npos) return;
    obj->setCanvasParent(nullptr);
    m_container.erase(m_container.begin() + index);
}
void CanvasObjectGroup::removeObject(const std::vector<CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
    for(size_t i=0; i<objs.size(); ++i)
    {
        removeObject(objs[i]);
    }
}
void CanvasObjectGroup::deleteObject(Objects::CanvasObject *obj)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
    removeObject(obj);
    delete obj;
}
void CanvasObjectGroup::deleteObject(const std::vector<Objects::CanvasObject*> &objs)
{
    QSFMLP_FUNCTION(QSFMLP_OBJECTGROUP_COLOR_1);
    removeObject(objs);
    for(size_t i=0; i<objs.size(); ++i)
    {
        delete objs[i];
    }
}
void CanvasObjectGroup::clearObjects()
{
    m_toAddContainer.clear();
    m_container.clear();
}

void CanvasObjectGroup::reserveObjectsCount(size_t size)
{
    m_container.reserve(size);
}
size_t CanvasObjectGroup::getObjectsCount() const
{
    return m_container.size();
}

const std::vector<CanvasObject*> &CanvasObjectGroup::getObjects() const
{
    return m_container;
}

bool CanvasObjectGroup::objectExists(CanvasObject *obj)
{
    return objectExists<CanvasObject>(obj,m_container);
}
size_t CanvasObjectGroup::getObjectIndex(CanvasObject *obj)
{
    return getObjectIndex<CanvasObject>(obj,m_container);
}
void CanvasObjectGroup::deleteLater(Objects::CanvasObject *obj)
{
    for(size_t i=0; i<m_toAddContainer.size(); ++i)
    {
        if(m_toAddContainer[i] == obj)
        {
            m_toAddContainer.erase(m_toAddContainer.begin() + i);
            break;
        }
    }
    m_toDelete.push_back(obj);
}

void CanvasObjectGroup::updateNewElements()
{
    deleteObject_internal();
    addObject_internal();
    for(size_t i=0; i<m_container.size(); ++i)
    {
        if(m_container[i]->m_objectsChanged)
            m_container[i]->updateNewElements();
    }
}
void CanvasObjectGroup::sfEvent(const std::vector<sf::Event> &events)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->sfEvent(events);
    }
}
void CanvasObjectGroup::update()
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->update_internal();
    }
}
void CanvasObjectGroup::draw(sf::RenderWindow &window)
{
    for(size_t i=0; i<m_container.size(); ++i)
    {
        m_container[i]->draw(window);
    }
}

